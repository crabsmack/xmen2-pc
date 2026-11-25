/* rewind.cpp - quick saves and rewind in a played run (main.cpp plat_at_sync; NOTES "Quick saves and
 * rewind").
 *
 * Both are the phase-7 savestates (state.cpp) used in a played run, in this process: nothing is
 * written to disk, so the slots are gone when the game quits (a state holds the game fiber's raw
 * stack, valid only in a process laid out as the saving one: state_place.cpp). They change nothing
 * the game does until a key is pressed, and are off in online play (rollback owns the state there).
 *
 *   F5 saves to the slot in use, F7 loads it, F6 picks the next slot (1-4); Rewind held rewinds
 *   (the Controls page's last row, controls.cpp CB_REWIND: by default Backspace and a controller's
 *   left trigger).
 *
 * Every keypress takes effect at the next sync point (the game's fiber is suspended there).
 *
 * The rewind ring: at every sync point the state is saved, and the ring keeps, for each runner
 * frame k, what turns S_k back into S_(k-1) (each section XORed with the newer one's, or the older
 * section whole when its length changed: the fiber's live stack), as runs of zero bytes and
 * literals, with the pads frame k ran with and S_k's canonical digest (machine_digest). The newest
 * state is kept whole. A rewind step drops the newest frame C, rebuilds S_(C-2), loads it and runs
 * frame C-1 again with its own pads: the picture of C-1 is drawn and shown as it was, the sound
 * chips run silent, and the state is S_(C-1) again. Its digest is checked against the one kept
 * (counters `rewind.mismatch`: 0, or the replay is not the game it was). The ring's oldest frames
 * go when it holds more than [rewind] seconds or 256 MB.
 *
 * XMEN2_HOTKEYS=STEP:ACTION,... (headless tests, tools/rewindcheck.py): at the STEP-th sync point
 * of the run (counting the rewind's own), save, load, slotN or rewindN (held for N steps); the
 * ring records in such a run as in a played one. XMEN2_REWIND_SKIP=SECTION: loads leave that
 * section out (the negative control).
 */
#include <chrono>
#include <deque>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>

#include "host_sdl.h"
#include "native.h"
#include "state.h"

namespace {

/* ---- the encoding: what turns a newer snapshot back into the older one ---------------------------- */

void put_var(std::vector<uint8_t> &o, uint64_t v)
{
    while (v >= 0x80) { o.push_back((uint8_t)(v | 0x80)); v >>= 7; }
    o.push_back((uint8_t)v);
}

bool get_var(const uint8_t *&p, const uint8_t *end, uint64_t *v)
{
    uint64_t r = 0;
    for (int s = 0; s < 64; s += 7) {
        if (p >= end) return false;
        uint8_t b = *p++;
        r |= (uint64_t)(b & 0x7F) << s;
        if (!(b & 0x80)) { *v = r; return true; }
    }
    return false;
}

/* `n` bytes of a ^ b (b null: a itself) as (zeros, literals, the literals) runs */
void put_runs(std::vector<uint8_t> &o, const uint8_t *a, const uint8_t *b, size_t n)
{
    static std::vector<uint8_t> x;
    x.resize(n);
    if (b)
        for (size_t i = 0; i < n; i++) x[i] = a[i] ^ b[i];
    else if (n)
        memcpy(x.data(), a, n);
    size_t i = 0;
    while (i < n) {
        size_t z = i;
        while (z < n && !x[z]) z++;
        size_t l = z;   /* literals end at the next run of 8 zeros (shorter ones cost less as literals) */
        while (l < n) {
            if (x[l]) { l++; continue; }
            size_t e = l;
            while (e < n && !x[e] && e - l < 8) e++;
            if (e - l >= 8 || e == n) break;
            l = e;
        }
        put_var(o, z - i);
        put_var(o, l - z);
        o.insert(o.end(), x.begin() + (long)z, x.begin() + (long)l);
        i = l;
    }
}

/* the runs into out (n bytes), XORed into what is there (xor) or over zeros */
bool get_runs(const uint8_t *&p, const uint8_t *end, uint8_t *out, size_t n, bool x)
{
    size_t i = 0;
    while (i < n) {
        uint64_t z, l;
        if (!get_var(p, end, &z) || !get_var(p, end, &l) || z > n - i || l > n - i - z || l > (uint64_t)(end - p)) return false;
        if (!x) memset(out + i, 0, (size_t)z);
        i += (size_t)z;
        if (x)
            for (size_t k = 0; k < l; k++) out[i + k] ^= p[k];
        else
            memcpy(out + i, p, (size_t)l);
        p += l;
        i += (size_t)l;
    }
    return true;
}

/* older given newer: per section its length and how it is kept (0: XOR with newer's, 1: whole) */
void encode(const Snapshot &older, const Snapshot &newer, std::vector<uint8_t> &o)
{
    o.clear();
    put_var(o, older.spans.size());
    for (size_t s = 0; s < older.spans.size(); s++) {
        size_t n = older.spans[s].second;
        const uint8_t *a = older.bytes.data() + older.spans[s].first;
        bool x = s < newer.spans.size() && newer.spans[s].second == n;
        put_var(o, n);
        o.push_back(x ? 0 : 1);
        put_runs(o, a, x ? newer.bytes.data() + newer.spans[s].first : nullptr, n);
    }
    size_t n = older.counters.size();
    bool x = newer.counters.size() == n;
    put_var(o, n);
    o.push_back(x ? 0 : 1);
    put_runs(o, older.counters.data(), x ? newer.counters.data() : nullptr, n);
}

bool decode(const Snapshot &newer, const std::vector<uint8_t> &d, Snapshot &older)
{
    const uint8_t *p = d.data(), *end = d.data() + d.size();
    uint64_t secs;
    if (!get_var(p, end, &secs) || secs > 4096) return false;
    older.spans.resize((size_t)secs);
    older.bytes.clear();
    for (size_t s = 0; s < secs; s++) {
        uint64_t n;
        if (!get_var(p, end, &n) || p >= end || n > (64u << 20)) return false;
        bool x = *p++ == 0;
        if (x && (s >= newer.spans.size() || newer.spans[s].second != n)) return false;
        size_t at = older.bytes.size();
        older.spans[s] = { at, (size_t)n };
        if (x)
            older.bytes.insert(older.bytes.end(), newer.bytes.begin() + (long)newer.spans[s].first,
                               newer.bytes.begin() + (long)(newer.spans[s].first + n));
        else
            older.bytes.resize(at + (size_t)n);
        if (!get_runs(p, end, older.bytes.data() + at, (size_t)n, x)) return false;
    }
    uint64_t n;
    if (!get_var(p, end, &n) || p >= end || n > (1u << 20)) return false;
    bool x = *p++ == 0;
    if (x && newer.counters.size() != n) return false;
    if (x) older.counters = newer.counters;
    else older.counters.assign((size_t)n, 0);
    return get_runs(p, end, older.counters.data(), (size_t)n, x) && p == end;
}

/* ---- the ring and the slots --------------------------------------------------------------------- */

struct Entry {
    std::vector<uint8_t> back;   /* S_k -> S_(k-1) */
    uint16_t pads[4];            /* frame k's */
    uint64_t digest;             /* S_k's */
    uint64_t frame;              /* k */
};

const size_t RING_CAP_BYTES = 256u << 20;
enum { SLOTS = 4 };

bool s_on;                 /* the ring records (a played run, or XMEN2_HOTKEYS) */
size_t s_max_frames;       /* [rewind] seconds as frames; 0 = no ring */
std::deque<Entry> s_ring;
size_t s_ring_bytes;
Snapshot s_top;            /* the newest state, whole (S of the ring's newest frame) */
uint64_t s_top_digest, s_top_frame;
bool s_have_top;
Snapshot s_slot[SLOTS];
bool s_slot_used[SLOTS];
int s_cur_slot;
const char *s_skip;        /* XMEN2_REWIND_SKIP */

/* requests from the keys (host_sdl.cpp) or XMEN2_HOTKEYS, taken at the next sync point */
bool s_want_save, s_want_load, s_want_slot, s_held;
bool s_was_held, s_said_end;

/* XMEN2_HOTKEYS */
struct Act { uint64_t step; std::string what; };
std::vector<Act> s_acts;
size_t s_act_next;
uint64_t s_step, s_hold_left;

/* the pads a replayed frame runs with (main.cpp plat_frame_begin) */
bool s_force;
uint16_t s_force_pads[4];

/* counters */
uint64_t c_pushed, c_push_bytes, c_push_ns, c_steps, c_mismatch, c_saves, c_loads, c_empty, c_dropped, c_max_bytes, c_bad;

void say(const char *fmt, int n = 0)
{
    char b[48];
    snprintf(b, sizeof b, fmt, n);
    if (plat_realtime()) host_osd(b);
    else fprintf(stderr, "[rewind] %s\n", b);
}

void ring_clear(void)
{
    s_ring.clear();
    s_ring_bytes = 0;
}

void trim(void)
{
    while (!s_ring.empty() && (s_ring.size() > s_max_frames || s_ring_bytes > RING_CAP_BYTES)) {
        s_ring_bytes -= s_ring.front().back.size();
        s_ring.pop_front();
        c_dropped++;
    }
}

/* the frame just run: into the ring */
void push(void)
{
    auto t0 = std::chrono::steady_clock::now();
    static Snapshot now;
    state_save(now);
    uint64_t dg = machine_digest(), f = machine_frame_no();
    if (s_have_top && s_max_frames) {
        Entry e;
        encode(s_top, now, e.back);
        e.back.shrink_to_fit();
        main_frame_pads(e.pads);
        e.digest = dg;
        e.frame = f;
        s_ring_bytes += e.back.size();
        c_push_bytes += e.back.size();
        s_ring.push_back(std::move(e));
        trim();
        if (s_ring_bytes > c_max_bytes) c_max_bytes = s_ring_bytes;
        c_pushed++;
    }
    std::swap(s_top, now);
    s_top_digest = dg;
    s_top_frame = f;
    s_have_top = true;
    c_push_ns += (uint64_t)std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - t0).count();
}

/* a state of frame `to` is loaded in place of frame `from`'s: the presentation's clock and
   interpolation follow (present.cpp), the sound queued for the old timeline goes */
void jumped(uint64_t from, uint64_t to)
{
    present_jump(from, to);
    if (plat_realtime()) host_audio_discard();
}

/* one step back from frame C: S_(C-2) loaded and frame C-1's pads set, so the host loop's next tick
   runs frame C-1 again (drawn and shown, its sound silent) and the state is S_(C-1), the ring's
   newest again (checked at the next sync point: step_done); false: no older frame */
bool s_replaying;   /* the next tick is a step's frame */

/* an entry that doesn't decode (never seen; counters rewind.bad): the ring starts again from the next
   frame, the game where it is */
bool broken(void)
{
    c_bad++;
    ring_clear();
    s_have_top = false;
    return false;
}

bool step_back(void)
{
    if (s_ring.size() < 2) return false;
    static Snapshot older, before;
    uint64_t from = machine_frame_no();
    if (!decode(s_top, s_ring.back().back, older)) return broken();
    s_ring_bytes -= s_ring.back().back.size();
    s_ring.pop_back();
    std::swap(s_top, older);   /* S_(C-1) */
    const Entry &e = s_ring.back();   /* frame C-1: its pads, S_(C-1)'s digest; S_(C-2) from its bytes */
    s_top_digest = e.digest;
    s_top_frame = e.frame;
    if (!decode(s_top, e.back, before)) return broken();
    state_load(before, s_skip);
    jumped(from, machine_frame_no());
    memcpy(s_force_pads, e.pads, sizeof s_force_pads);
    s_force = s_replaying = true;
    return true;
}

void step_done(void)
{
    s_force = s_replaying = false;
    c_steps++;
    if (machine_digest() != s_top_digest || machine_frame_no() != s_top_frame) {
        c_mismatch++;
        if (c_mismatch == 1)
            fprintf(stderr, "[rewind] frame %llu replayed is not the frame kept (digest %016llx, kept %016llx)\n",
                    (unsigned long long)machine_frame_no(), (unsigned long long)machine_digest(),
                    (unsigned long long)s_top_digest);
    }
}

void do_save(void)
{
    state_save(s_slot[s_cur_slot]);
    s_slot_used[s_cur_slot] = true;
    c_saves++;
    say("SAVED TO SLOT %d", s_cur_slot + 1);
}

void do_load(void)
{
    if (!s_slot_used[s_cur_slot]) {
        c_empty++;
        say("SLOT %d IS EMPTY", s_cur_slot + 1);
        return;
    }
    uint64_t from = machine_frame_no();
    state_load(s_slot[s_cur_slot], s_skip);
    jumped(from, machine_frame_no());
    ring_clear();   /* the ring's frames are another timeline's: it starts again here, from the slot's */
    s_top = s_slot[s_cur_slot];
    s_top_digest = machine_digest();
    s_top_frame = machine_frame_no();
    s_have_top = true;
    c_loads++;
    say("LOADED SLOT %d", s_cur_slot + 1);
}

void next_slot(void)
{
    s_cur_slot = (s_cur_slot + 1) % SLOTS;
    say(s_slot_used[s_cur_slot] ? "SLOT %d" : "SLOT %d (EMPTY)", s_cur_slot + 1);
}

void script_actions(void)
{
    s_step++;
    if (s_hold_left) s_hold_left--;
    while (s_act_next < s_acts.size() && s_acts[s_act_next].step <= s_step) {
        const std::string &w = s_acts[s_act_next++].what;
        if (w == "save") s_want_save = true;
        else if (w == "load") s_want_load = true;
        else if (w.rfind("slot", 0) == 0) { s_cur_slot = (atoi(w.c_str() + 4) - 1 + SLOTS) % SLOTS; }
        else if (w.rfind("rewind", 0) == 0) s_hold_left = strtoull(w.c_str() + 6, nullptr, 10);
    }
    s_held = s_hold_left > 0;
}

}  // namespace

void rewind_init(bool played)
{
    const char *acts = getenv("XMEN2_HOTKEYS");
    s_on = (played && !netplay_enabled()) || (acts && *acts);
    if (!s_on) return;
    int seconds = atoi(options_ini_get("rewind", "seconds", "60").c_str());
    if (const char *t = getenv("XMEN2_REWIND_SECONDS"); t && *t) seconds = atoi(t);
    if (seconds < 0) seconds = 0;
    if (seconds > 3600) seconds = 3600;
    s_max_frames = (size_t)seconds * 60;
    if (const char *t = getenv("XMEN2_REWIND_SKIP"); t && *t) {
        if (!state_has_section(t)) { fprintf(stderr, "[rewind] XMEN2_REWIND_SKIP: no section %s\n", t); exit(2); }
        s_skip = t;
    }
    for (const char *s = acts; s && *s;) {
        char *e;
        unsigned long long step = strtoull(s, &e, 10);
        if (e == s || *e != ':') { fprintf(stderr, "[rewind] XMEN2_HOTKEYS: STEP:ACTION,... (%s)\n", s); exit(2); }
        const char *w = e + 1, *c = strchr(w, ',');
        std::string what = c ? std::string(w, (size_t)(c - w)) : std::string(w);
        s_acts.push_back({ step, what });
        s = c ? c + 1 : w + what.size();
    }
}

bool rewind_on(void) { return s_on; }

/* host_sdl.cpp's keys (a played run) */
void rewind_key_save(void) { s_want_save = true; }
void rewind_key_load(void) { s_want_load = true; }
void rewind_key_slot(void) { s_want_slot = true; }
void rewind_key_held(bool held) { if (s_acts.empty()) s_held = held; }

bool rewind_pads(uint16_t pads[4])
{
    if (!s_force) return false;
    memcpy(pads, s_force_pads, sizeof s_force_pads);
    return true;
}

bool rewind_replaying(void) { return s_replaying; }

/* main.cpp plat_at_sync: the game is at a sync point, its frame done; the host loop runs the next
   frame when this returns */
void rewind_at_sync(void)
{
    if (!s_on) return;
    if (s_replaying) step_done();   /* the frame just run was a rewind step's: the ring's newest again */
    else if (s_max_frames) push();  /* else it goes in the ring first (a rewind begun now starts from it) */
    if (!s_acts.empty()) script_actions();
    if (s_want_slot) { s_want_slot = false; next_slot(); }
    if (s_want_save) { s_want_save = false; do_save(); }
    if (s_want_load) { s_want_load = false; do_load(); }
    if (s_held && s_max_frames) {
        if (!s_was_held) { s_was_held = true; s_said_end = false; if (plat_realtime()) host_audio_discard(); }
        if (step_back()) {
            if (plat_realtime()) host_osd("<< REWIND");
            return;
        }
        if (!s_said_end) { s_said_end = true; say("REWIND: NO OLDER FRAMES"); }
        /* held at the oldest frame: the picture stays and the game waits here until it is let go */
        while (s_held) {
            main_hold_picture();
            if (!s_acts.empty()) script_actions();
        }
    }
    s_was_held = false;
}

std::string rewind_counters_json(void)
{
    char b[512];
    snprintf(b, sizeof b,
             "{\"on\": %s, \"seconds\": %llu, \"frames\": %llu, \"bytes\": %llu, \"max_bytes\": %llu, \"pushed\": %llu, "
             "\"bytes_per_frame\": %llu, \"push_us\": %.1f, \"dropped\": %llu, \"steps\": %llu, \"mismatch\": %llu, "
             "\"bad\": %llu, \"saves\": %llu, \"loads\": %llu, \"empty_loads\": %llu}",
             s_on ? "true" : "false", (unsigned long long)(s_max_frames / 60), (unsigned long long)s_ring.size(),
             (unsigned long long)s_ring_bytes, (unsigned long long)c_max_bytes, (unsigned long long)c_pushed,
             (unsigned long long)(c_pushed ? c_push_bytes / c_pushed : 0),
             (c_pushed + 1) ? (double)c_push_ns / 1000.0 / (double)(c_pushed + 1) : 0.0, (unsigned long long)c_dropped,
             (unsigned long long)c_steps, (unsigned long long)c_mismatch, (unsigned long long)c_bad,
             (unsigned long long)c_saves, (unsigned long long)c_loads, (unsigned long long)c_empty);
    return b;
}

/* the encoding's own check (XMEN2_REWIND_SELFTEST=1, rewindcheck.py): random sections, some lengths
   changed, decoded back */
int rewind_selftest(void)
{
    uint64_t r = 12345;
    auto rnd = [&]() { r ^= r << 13; r ^= r >> 7; r ^= r << 17; return r; };
    int bad = 0;
    for (int t = 0; t < 200; t++) {
        Snapshot a, b;
        for (Snapshot *s : { &a, &b }) {
            s->bytes.clear();
            s->spans.clear();
        }
        int secs = 1 + (int)(rnd() % 6);
        for (int s = 0; s < secs; s++) {
            size_t na = (size_t)(rnd() % 3000), nb = rnd() % 3 ? na : (size_t)(rnd() % 3000);
            size_t at = a.bytes.size(), bt = b.bytes.size();
            a.spans.push_back({ at, na });
            b.spans.push_back({ bt, nb });
            for (size_t i = 0; i < na; i++) a.bytes.push_back(rnd() % 4 ? 0 : (uint8_t)rnd());
            for (size_t i = 0; i < nb; i++) b.bytes.push_back(i < na && rnd() % 5 ? a.bytes[at + i] : (uint8_t)rnd());
        }
        a.counters.assign(24, 7);
        b.counters.assign(t % 2 ? 24 : 16, 9);
        std::vector<uint8_t> d;
        encode(a, b, d);
        Snapshot back;
        if (!decode(b, d, back) || back.bytes != a.bytes || back.spans != a.spans || back.counters != a.counters) bad++;
        d[d.size() / 2] ^= 0x5A;   /* a damaged entry must not decode to the same state */
        if (decode(b, d, back) && back.bytes == a.bytes && back.spans == a.spans && back.counters == a.counters) bad++;
    }
    fprintf(stderr, "[rewind] selftest: %d of 400 checks failed\n", bad);
    return bad ? 1 : 0;
}
