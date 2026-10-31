/* sample_retime.h - the options sample_retime and sample_smooth (both on by default; the user,
 * 2026-10-30: "default on", "likewise, default on, separate option"): each sound machine's DAC
 * samples played at a steady rate, and with linear ramps between them (sample_retime.cpp). Sound
 * only: what the driver writes, its Z80 and the chips' registers as the game sees them are
 * unchanged; only when and how each sample reaches the chip.
 */
#ifndef XMEN2_SAMPLE_RETIME_H
#define XMEN2_SAMPLE_RETIME_H

#include <stddef.h>
#include <stdint.h>

#include <vector>

#include "xmen2_options.h"

/* a chip write of a machine's frame: master clocks from the Z80 frame's start, the port (0-3 FM, 4
   PSG), the value (sound2.cpp's machines keep their frame's in these) */
struct ChipEv { uint32_t stamp; uint8_t port, v; };

#define DAC_PENDING 1024   /* samples not yet out: at most 4 ms of them (the counters' pending_max) */

/* One machine's retimer: plain data, in its snapshot (a rollback's replay and a savestate retime the
   same way). Times are master clocks since the retimer started, on the Z80's frames. */
struct DacRetime {
    uint64_t frame;               /* the frames merged so far: this frame starts at frame * DAC_FRAME */
    double   period;              /* the sample's spacing: its last values' span over their count */
    double   last_in, last_out;   /* the last value: when the driver wrote it, when it goes out */
    double   hist[128];           /* when the driver wrote the sample's last values, a ring */
    int32_t  nhist, ihist;
    /* the chip's address latch, one for both parts (port 0: the address, port 2: $100 | it; a data
       write of port 1 is ignored while it has $100, ymfm's ym2612::write_data): as the driver's writes
       leave it (in order), and as the merged writes leave it */
    uint16_t in_addr, out_addr;
    uint32_t head, count;         /* the samples waiting, a ring by time */
    double   at[DAC_PENDING];
    uint8_t  val[DAC_PENDING];
    uint64_t retimed, dropped;    /* samples moved; samples lost to a full ring */
    uint32_t pending_max;         /* the most waiting at once */
    uint32_t pad2;
    /* sample_smooth: the last value written to the chip, and the next step of the ramp towards the
       one waiting (1: the first) */
    double   emit_at;
    int32_t  ramp_k;
    uint8_t  emit_val, pad3[3];
    uint64_t ramp_steps;          /* steps written */
};
#define DAC_FRAME ((uint64_t)262 * 3420)   /* master clocks of a Z80 frame (machine.cpp's) */

/* sample_retime or sample_smooth: the samples go through the retimer (dac_take, dac_merge) */
static inline bool dac_on(void) { return g_opt.sample_retime || g_opt.sample_smooth; }

/* The first machine's (main.cpp): its retimer and its frame's FM writes so far, which wait for the
   frame's end there (the Z80 runs between frames too, when the 68000 gives its bus back, so a snapshot
   can find some). Both in machine.cpp's "sound". */
extern DacRetime g_dac_m1;
extern std::vector<ChipEv> g_fm_m1;
size_t dac_m1_save(uint8_t *dst, size_t cap);   /* dst null: the bytes it needs */
int    dac_m1_load(const uint8_t *src, size_t len);

void dac_reset(DacRetime *d);
/* A write of the frame, in the driver's order: true when it was a DAC sample (taken: not to the
   chip now, dac_merge puts it in), false for every other write (the caller keeps it as it is) */
bool dac_take(DacRetime *d, const char *who, uint32_t stamp, unsigned port, uint8_t v);
/* The frame's other FM writes `in` (in order) with the samples due before the next frame merged in by
   time (sample_smooth: and the ramps' steps leading to them); each is a select of $2A, the value and,
   unless it was $2A, the address the chip had (on its part's port). The PSG's writes pass through.
   Ends the frame. */
void dac_merge(DacRetime *d, const ChipEv *in, size_t n, std::vector<ChipEv> *out);

#endif
