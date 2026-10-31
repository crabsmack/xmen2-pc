/* sample_retime.cpp - the option sample_retime: each sound machine's DAC samples played at a steady
 * rate (the user, 2026-10-30, after the A/B clips: "1/2 yes unless there's a cost, let's have it,
 * and default on").
 *
 * The GEMS driver writes a sample's values to the YM2612's DAC ($2A) from its Z80 loop, and in this
 * machine they reach the chip unevenly: over the 23 stage inputs 70% of the gaps within 5% of the
 * sample's spacing, 98% retimed (docs/NOTES.md "sample_retime"). The retimer is a phase-locked loop
 * per machine: a sample's spacing is its last values' span over their count (up to 128; a mean of
 * the gaps that leaves the outliers out locked onto a sample's first gap, often a burst), each
 * value goes out one spacing after the last, pulled 1/256 of the way back towards 2 ms after the
 * driver wrote it, and never earlier than written or more than 4 ms later. A gap of more than
 * 30,000 master clocks (0.56 ms) starts a new sample (its own rate): its first value goes out 2 ms
 * after it was written. The values and their order are the driver's; only their times move.
 *
 * sample_smooth: the chip holds a DAC value until the next (a stair-step, whose images sound as
 * high frequencies); with the option each value is reached by a straight line from the last, one
 * value every chip sample (1,008 master clocks), none across a gap that starts a sample. Measured on
 * the DAC alone (NOTES "sample_smooth"): 5-9 dB less of its energy above 4 kHz. With sample_retime
 * off the lines lead to the values at the driver's own times; a line begun in the frame before its
 * value was written is put at the start of that value's frame.
 *
 * Nothing waits a frame: a frame's samples due by its end go to the chip with it (merged into its
 * other writes by time), the rest with the next. The chip has one address latch for both parts:
 * each value is written as a select of $2A and the value, and the address the chip had (either
 * part's) is selected again, so a write that relied on it still lands there.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sample_retime.h"
#include "native.h"

#define MASTER_HZ   53693175.0
#define LATE_MAX    (4e-3 * MASTER_HZ)   /* the most a value goes out after it was written */
#define NEW_SAMPLE  30000.0              /* a longer gap starts a sample */
#define PULL        256.0                /* how slowly a value is pulled back towards LATE_MAX / 2 */

DacRetime g_dac_m1 = [] { DacRetime d{}; dac_reset(&d); return d; }();
std::vector<ChipEv> g_fm_m1;

size_t dac_m1_save(uint8_t *dst, size_t cap)
{
    uint32_t n = (uint32_t)g_fm_m1.size();
    size_t need = sizeof g_dac_m1 + sizeof n + n * sizeof(ChipEv);
    if (!dst) return need;
    if (cap < need) return 0;
    memcpy(dst, &g_dac_m1, sizeof g_dac_m1);
    memcpy(dst + sizeof g_dac_m1, &n, sizeof n);
    if (n) memcpy(dst + sizeof g_dac_m1 + sizeof n, g_fm_m1.data(), n * sizeof(ChipEv));
    return need;
}

int dac_m1_load(const uint8_t *src, size_t len)
{
    uint32_t n;
    if (len < sizeof g_dac_m1 + sizeof n) return 0;
    memcpy(&n, src + sizeof g_dac_m1, sizeof n);
    if (len != sizeof g_dac_m1 + sizeof n + (size_t)n * sizeof(ChipEv)) return 0;
    memcpy(&g_dac_m1, src, sizeof g_dac_m1);
    const ChipEv *e = (const ChipEv *)(src + sizeof g_dac_m1 + sizeof n);
    g_fm_m1.assign(e, e + n);
    return 1;
}

/* a machine (re)started: nothing waiting, no sample yet; the counts go on */
void dac_reset(DacRetime *d)
{
    uint64_t retimed = d->retimed, dropped = d->dropped, steps = d->ramp_steps;
    uint32_t most = d->pending_max;
    memset(d, 0, sizeof *d);
    d->period = 10155;                  /* unused until a sample has two values */
    d->last_in = d->last_out = -1e18;
    d->emit_at = -1e18;
    d->ramp_k = 1;
    d->emit_val = 0x80;
    d->retimed = retimed;
    d->dropped = dropped;
    d->pending_max = most;
    d->ramp_steps = steps;
}

/* XMEN2_DAC_LOG=FILE (dev): every value taken, `machine,in,out,value`: when the driver wrote it and
   when it goes out (the ramps' steps not listed), master clocks on that machine's retimer's clock. A
   replayed frame is logged again. */
static FILE *dac_log(void)
{
    static FILE *f = [] {
        const char *p = getenv("XMEN2_DAC_LOG");
        FILE *o = p && *p ? fopen(p, "w") : nullptr;
        if (p && *p && !o) { fprintf(stderr, "[sound] cannot write %s\n", p); exit(2); }
        return o;
    }();
    return f;
}

bool dac_take(DacRetime *d, const char *who, uint32_t stamp, unsigned port, uint8_t v)
{
    if (port == 0 || port == 2) d->in_addr = (uint16_t)(port << 7 | v);
    if (port != 1 || d->in_addr != 0x2A) return false;
    double in = (double)(d->frame * DAC_FRAME + stamp), out, gap = in - d->last_in;
    if (gap > NEW_SAMPLE) d->nhist = d->ihist = 0;
    d->hist[d->ihist] = in;
    d->ihist = (d->ihist + 1) & 127;
    if (d->nhist < 128) d->nhist++;
    if (!g_opt.sample_retime) out = in;   /* sample_smooth alone: the driver's times */
    else if (d->nhist == 1) out = in + LATE_MAX / 2;
    else {
        d->period = (in - d->hist[(d->ihist - d->nhist) & 127]) / (d->nhist - 1);
        out = d->last_out + d->period;
        out += (in + LATE_MAX / 2 - out) / PULL;
        if (out > in + LATE_MAX) out = in + LATE_MAX;
        if (out < in) out = in;
    }
    if (out < d->last_out) out = d->last_out;   /* in order */
    d->last_in = in;
    d->last_out = out;
    if (d->count == DAC_PENDING) {
        d->dropped++;
        return true;
    }
    uint32_t i = (d->head + d->count) % DAC_PENDING;
    d->at[i] = out;
    d->val[i] = v;
    if (++d->count > d->pending_max) d->pending_max = d->count;
    d->retimed++;
    if (FILE *f = dac_log()) fprintf(f, "%s,%.0f,%.0f,%u\n", who, in, out, v);
    return true;
}

/* sample_smooth (the user, 2026-10-30: "likewise, default on, separate option"): between the last
   value written and the next, a value every chip sample (1,008 master clocks), on the straight line
   from the one to the other; none across a gap that starts a sample */
#define CHIP_SAMPLE 1008.0

void dac_merge(DacRetime *d, const ChipEv *in, size_t n, std::vector<ChipEv> *out)
{
    const double base = (double)(d->frame * DAC_FRAME), end = base + (double)DAC_FRAME;
    /* the next write: a step of the ramp towards the value waiting (1), the value (2), or none (0) */
    auto next = [&](double *t, uint8_t *v) -> int {
        if (!d->count) return 0;
        double at = d->at[d->head];
        uint8_t to = d->val[d->head];
        if (g_opt.sample_smooth && at - d->emit_at < NEW_SAMPLE) {
            int m = (int)((at - d->emit_at) / CHIP_SAMPLE);
            if (d->ramp_k < m) {
                *t = d->emit_at + d->ramp_k * CHIP_SAMPLE;
                *v = (uint8_t)(d->emit_val + ((int)to - (int)d->emit_val) * d->ramp_k / m);
                return 1;
            }
        }
        *t = at;
        *v = to;
        return 2;
    };
    auto stamp_of = [&](double t) { t -= base; return (uint32_t)(t < 0 ? 0 : t); };
    /* the writes due before `limit` (a stamp of this frame) */
    auto put = [&](double limit) {
        double t;
        uint8_t v;
        for (int k; (k = next(&t, &v)) && t < end && stamp_of(t) < limit;) {
            uint32_t s = stamp_of(t);
            /* the select every time: left out when the address was already $2A, the sound changed with
               sample_smooth on (not with sample_retime alone; the cause wasn't found) */
            out->push_back({ s, 0, 0x2A });
            out->push_back({ s, 1, v });
            if (d->out_addr != 0x2A) out->push_back({ s, (uint8_t)(d->out_addr & 0x100 ? 2 : 0), (uint8_t)d->out_addr });
            if (k == 1) {
                d->ramp_k++;
                d->ramp_steps++;
                continue;
            }
            d->emit_at = t;
            d->emit_val = v;
            d->ramp_k = 1;
            d->head = (d->head + 1) % DAC_PENDING;
            d->count--;
        }
    };
    for (size_t k = 0; k < n; k++) {
        const ChipEv &e = in[k];
        if (e.port <= 3) put((double)e.stamp);
        out->push_back(e);
        if (e.port == 0 || e.port == 2) d->out_addr = (uint16_t)(e.port << 7 | e.v);
    }
    put(1e30);
    d->frame++;
}
