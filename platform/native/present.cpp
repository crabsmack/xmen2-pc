/* present.cpp - the native runner's display clock and high frame rate (xmen2disasm
 * docs/phases/P6-enhance.md R4, E4, "The contract": --present-log, the `present` counters).
 *
 * The logic stays the 60 Hz frame-synchronous machine. Runner frame k's picture R_k is rendered at
 * sync point k-1, right after the V-int, and shown at k x T (T = 16,688.9 us, one NTSC frame), as
 * the 60 Hz build shows it. With high_fps = H, n - 1 frames evenly between (k-1) x T and k x T
 * (n = H x T rounded: 2 at 120 and 144, 3 at 165, 4 at 240) are interpolated from R_(k-1) toward
 * R_k and shown in between: R_k is already
 * rendered when they are made (the logic of runner frame k-1 is done and its pads were read when
 * the 60 Hz build reads them), so nothing waits for anything: no added latency, the in-between
 * frames only show earlier what R_k will show.
 *
 * An interpolated frame is R_k's VDP state rendered with three things moved back toward R_(k-1):
 * the horizontal scroll of every line (the camera and the per-line parallax), the vertical scroll,
 * and every sprite by its sprite instance's step (the emitter's anchor, plat_spr_anchor), else with
 * its owner (the draw-list node, timer or scene hook whose call wrote it, plat_spr_owner; both from
 * the game's native branches), all of an owner's sprites by the one step that lines up most of its
 * pieces with R_(k-1)'s, else (particles) each piece by its own short step. An instance not drawn
 * in R_(k-1) (a blinking player) moves from where it was last drawn, up to 9 frames back. Not
 * moved: the HUD's sprites, unmatched ones, a scroll step over 64 px or a sprite step over 48 px
 * (they snap), the palettes, the planes' tiles (the audit of 2026-11-18 found no motion drawn by
 * rewriting them: XMEN2_PRESENT_AUDIT).
 * Presentation only: each interpolated frame is rendered from a copy of the VDP, on worker threads
 * ("the frames in between, rendered on worker threads").
 */
#include <algorithm>
#include <chrono>
#include <cmath>
#include <condition_variable>
#include <map>
#include <memory>
#include <mutex>
#include <thread>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <unordered_map>
#include <vector>

extern "C" {
#include "video/genesis_machine.h"
}
#include "host_sdl.h"
#include "native.h"

#define T_US 16688.9          /* one NTSC frame: the contract's present-clock unit */
#define SNAP_SCROLL 64        /* px: a scroll step this big is a cut, not motion */
#define SNAP_SPRITE 48        /* px */

/* ---- the game's memory (68000 view) --------------------------------------------------------- */

static uint8_t m8(uint32_t a)
{
    a &= 0xFFFFFFu;
    if (a < 0x400000u) return g_rom[a];
    if (a >= 0xFF0000u) return xm_ram_peek8(a & 0xFFFFu);
    return 0;
}
static int16_t s16at(uint32_t a) { return (int16_t)(m8(a) << 8 | m8(a + 1)); }

/* ---- sprite owners (the game's native branches) --------------------------------------------- */

/* The sprite tables (phase 7): the main one, and with three or more players players 3-4's own. The
   main one is the VDP's 80 slots, or with three or more players 160 in players 3-4's region (the
   hardware shows the first 80, players.cpp draws the rest); players 3-4's are in the region too
   (P34_SPR_TABLE, drawn by players.cpp). Each fills in order from slot 0. */
#define NT 3          /* 0 the main table, 1 player 3's, 2 player 4's */
#define MAXS 160      /* the main table's slots with three or more players (xmen2_mem.h SPR_STOP) */
extern "C" uint8_t xm_region[];
static uint32_t rg_off(int t) { return t ? 0x0400u + 0x300u * (uint32_t)(t - 1) : 0x1000u; }   /* P34_SPR_TABLE(k), P34_SPR_MAIN */
static int slots(int t) { return !t && g_opt.players > 2 ? MAXS : 80; }
static int none_index(int t) { return !t && g_opt.players > 2 ? MAXS + 1 : 0x51; }   /* SPR_START: none written yet */

/* While runner frame k-1's logic builds the sprite lists (DMA'd at the next V-int: R_k's), each
   owner's draw is announced with the slot written last before it (g_SprIndex, $FFE21E, its table's
   while a player 3-4's code runs: P34_Enter swaps it); slots are filled in order from 0
   (Spr_BeginFrame). Until 2026-10-31 a player 3-4's marks went to the main table with its own
   table's slot numbers: main-table sprites took that player's steps (player 2's health bar slid
   while player 3 moved, in the playtest of that day). */
namespace { struct Mark { uintptr_t owner; int before; uintptr_t spr; int ax, ay; }; }   /* spr: an anchor mark (internal linkage: players.cpp has a Mark too) */
static std::vector<Mark> s_marks[NT];
static int s_ctx;                  /* the player 3-4 whose code runs (plat_p34_enter), else 0 */
static uintptr_t s_last_owner;     /* the owner announced last outside them */

static int table_now(void) { return s_ctx ? s_ctx - 2 : 0; }
static int index_now(int t)   /* table t's g_SprIndex */
{
    if (!t || s_ctx == t + 2) return s16at(0xFFE21E);
    uint32_t o = 0x0200u + 40u * (uint32_t)(t - 1) + 30u;   /* g_PX[t - 1].sprIndex (xmen2_players34.h PlayerX) */
    return (int16_t)(xm_region[o] << 8 | xm_region[o + 1]);
}
static void push_mark(int t, uintptr_t owner, uintptr_t spr, int ax, int ay)
{
    int i = index_now(t);
    s_marks[t].push_back({ owner, i == none_index(t) ? -1 : i, spr, ax, ay });
}

extern "C" void plat_spr_owner(const void *owner)
{
    players_spr_owner(owner, s16at(0xFFE21E));   /* phase 7: the mp counters' `drawn` */
    if (!g_opt.high_fps) return;
    if (!s_ctx) s_last_owner = (uintptr_t)owner;
    push_mark(table_now(), (uintptr_t)owner, 0, 0, 0);
}

/* Each sprite emitter (Spr_EmitPre/Stream and their Clip and Linked forms) announces the sprite
   instance whose pieces it writes next and where it draws it: within its owner's slots. */
extern "C" void plat_spr_anchor(const void *spr, long x, long y)
{
    if (!g_opt.high_fps) return;
    push_mark(table_now(), 0, (uintptr_t)spr, (int)x, (int)y);
}

/* players.cpp's plat_p34_enter: a player 3-4's code starts (k) or ends (0). The owner announced just
   before (its timer, its draw-list node, the hits: each marks, then enters) owns what it writes. */
void present_p34_enter(int k)
{
    s_ctx = k;
    if (k && g_opt.high_fps) push_mark(k - 2, s_last_owner, 0, 0, 0);
}

struct Tag { uintptr_t owner; int piece; uintptr_t spr; int ax, ay; };
static Tag s_tags[NT][MAXS], s_prev_tags[NT][MAXS];
namespace { struct Seen { int ax, ay, shift; uint64_t k; }; }   /* seen_update: an instance's last anchor */
static std::unordered_map<uintptr_t, Seen> s_seen[NT];

static void resolve_tags(int t)
{
    Tag *tags = s_tags[t];
    for (int s = 0; s < MAXS; s++) tags[s] = { 0, 0, 0, 0, 0 };
    int last = index_now(t), n = slots(t);
    last = last == none_index(t) ? -1 : last;
    std::vector<Mark> &marks = s_marks[t];
    static const char *dbg = getenv("XMEN2_PRESENT_DEBUG");
    if (!t && dbg && strtoull(dbg, nullptr, 10) == machine_frame_no()) {
        fprintf(stderr, "[present] frame %llu last %d marks", (unsigned long long)machine_frame_no(), last);
        for (const Mark &m : marks) fprintf(stderr, " (%llx,%d)", (unsigned long long)m.owner, m.before);
        fprintf(stderr, "\n");
        for (int s = 0; s <= last && s < 80; s++) {
            uint32_t e = 0xFFE226u + 8u * (uint32_t)s;
            fprintf(stderr, "[present]  slot %d attr %04X x %d y %d size %X\n", s, (unsigned)(uint16_t)s16at(e + 4),
                    s16at(e + 6), s16at(e), m8(e + 2));
        }
    }
    Tag cur = { 0, 0, 0, 0, 0 };
    for (size_t m = 0; m < marks.size(); m++) {
        const Mark &k = marks[m];
        if (k.spr) { cur.spr = k.spr; cur.ax = k.ax; cur.ay = k.ay; }      /* an anchor: same owner */
        else cur = { k.owner, 0, 0, 0, 0 };                                /* a new owner, no anchor yet */
        int from = k.before + 1;
        int to = m + 1 < marks.size() ? marks[m + 1].before : last;
        for (int slot = from; slot <= to && slot < n; slot++, cur.piece++)
            if (slot >= 0) tags[slot] = cur;
    }
    marks.clear();
}

/* ---- the VDP inputs an interpolation moves --------------------------------------------------- */

struct Snap {
    bool valid;
    uint8_t hscroll[0x400];       /* the horizontal scroll table (224 lines x A, B) */
    uint16_t vsram[GVDP_VSRAM_ENTRIES];
    uint8_t sat[80 * 8];          /* VRAM's attribute table: x at screen places (widescreen.cpp shifted it) */
    uint8_t rg[NT][MAXS * 8];     /* with three or more players the region's tables (the main one's slots 80
                                     on, players 3-4's): x unshifted, players.cpp shifts as it draws */
    bool live[NT];                /* the table is drawn this frame (players 3-4's: players_p34_tables_shown) */
    int shift;                    /* ws_render_shift(): anchors and the region's x are unshifted */
    uint8_t reg11, reg5, reg13;
};
static Snap s_prev, s_cur;

static uint32_t hbase(const GVDP *v) { return (uint32_t)(v->reg[13] & 0x3F) << 10; }
static uint32_t satbase(const GVDP *v) { return (uint32_t)(v->reg[5] & 0x7F) << 9; }

static void snap_now(Snap *s)
{
    const GVDP *v = &g_machine.vdp;
    uint32_t hb = hbase(v), sb = satbase(v);
    for (int i = 0; i < 0x400; i++) s->hscroll[i] = v->vram[(uint16_t)(hb + (uint32_t)i)];
    memcpy(s->vsram, v->vsram, sizeof s->vsram);
    for (int i = 0; i < 80 * 8; i++) s->sat[i] = v->vram[(uint16_t)(sb + (uint32_t)i)];
    s->shift = ws_render_shift();
    s->live[0] = true;
    for (int t = 1; t < NT; t++) {
        uint32_t o = 0x0200u + 40u * (uint32_t)(t - 1) + 36u;   /* g_PX[t - 1].sprLast: $51 = nothing written */
        s->live[t] = t + 2 <= g_opt.players && players_p34_tables_shown()
                     && (int16_t)(xm_region[o] << 8 | xm_region[o + 1]) != 0x51;
    }
    if (g_opt.players > 2)
        for (int t = 0; t < NT; t++) memcpy(s->rg[t], &xm_region[rg_off(t)], (size_t)slots(t) * 8);
    s->reg11 = v->reg[11];
    s->reg5 = v->reg[5];
    s->reg13 = v->reg[13];
    s->valid = true;
}

static int w16(const uint8_t *p) { return (int16_t)(p[0] << 8 | p[1]); }
static void put16(uint8_t *p, int v) { p[0] = (uint8_t)(v >> 8); p[1] = (uint8_t)v; }

/* slot s of table t in a snapshot (VRAM's for the main table's first 80, else the region's), and what
   its x needs added to be a screen place in VRAM's terms: the frame's shift for the region's */
static const uint8_t *ent(const Snap &n, int t, int s) { return !t && s < 80 ? n.sat + s * 8 : n.rg[t] + s * 8; }
static int xoff(const Snap &n, int t, int s) { return !t && s < 80 ? 0 : -n.shift; }
static int sx(const Snap &n, int t, int s) { return w16(ent(n, t, s) + 6) + xoff(n, t, s); }

/* a wrapped scroll value moved fraction a of the way from p to c (or c: a cut) */
static int lerp_wrap(int p, int c, double a, int mod)
{
    int d = ((c - p) % mod + mod + mod / 2) % mod - mod / 2;
    if (d > SNAP_SCROLL || d < -SNAP_SCROLL) return c;
    int v = p + (int)(a * d + (d >= 0 ? 0.5 : -0.5));
    return ((v % mod) + mod) % mod;
}

static bool hud_attr(int attr)
{
    static const int hud[] = { 0xA7E8, 0xA7F8, 0xA7FC, 0xA7D0, 0xA7E0, 0xA7D4 };
    for (int a : hud)   /* either flip: player 2's strips in the players' order are mirrored ($AFE0, P34_HudP2) */
        if ((attr & 0xE7FF) == a) return true;
    return false;
}

/* ---- counters, the log ---------------------------------------------------------------------- */

static FILE *s_log;
static uint64_t s_frames, s_whole, s_sprites, s_matched, s_unmatched[5], s_fallback;
static std::vector<uint32_t> s_render_us;
static bool s_gameplay;     /* the frame being led up to is a gameplay frame */

/* --flip-scan (main.cpp): a flipped pass's rows go to its file, after the flip's frame, and count
   for nothing */
static FILE *s_scan;
static uint64_t s_scan_flip;

/* --flip-scan-first: each flipped pass's rows are compared with the unflipped run's own rows as the
   run reaches them (sync N+L); the first flip N whose sync point N is a gameplay one and whose pass
   shows another picture at some sync point ends the scan (phase 6 E4 picks that N the same way,
   against the unflipped run's present log) */
static bool s_first;
static uint64_t s_first_len, s_found;
static std::map<uint64_t, std::pair<uint64_t, bool>> s_main;   /* sync -> (hash, gameplay) */
static std::vector<std::pair<uint64_t, std::vector<std::pair<uint64_t, uint64_t>>>> s_trials;   /* flip -> (sync, hash) */

void present_scan(FILE *f, uint64_t flip)
{
    s_scan = f;
    s_scan_flip = flip;
    if (s_first && f && !s_found) s_trials.push_back({flip, {}});
}

void present_scan_first(uint64_t len) { s_first = true; s_first_len = len; }
uint64_t present_scan_found(void) { return s_found; }

static void scan_resolve(uint64_t sync)   /* the unflipped run has reached `sync` */
{
    while (!s_trials.empty() && s_trials.front().first + s_first_len <= sync) {
        const auto &t = s_trials.front();
        auto at = s_main.find(t.first);
        bool changed = false;
        for (const auto &r : t.second) {
            auto m = s_main.find(r.first);
            if (m != s_main.end() && m->second.first != r.second) { changed = true; break; }
        }
        if (changed && at != s_main.end() && at->second.second) {
            s_found = t.first;
            s_trials.clear();
            s_main.clear();
            return;
        }
        s_trials.erase(s_trials.begin());
    }
    uint64_t keep = s_trials.empty() ? sync + 1 : s_trials.front().first;
    while (!s_main.empty() && s_main.begin()->first < keep) s_main.erase(s_main.begin());
}

extern "C" void present_note_render(double us)
{
    if (s_scan) return;
    if (g_opt.high_fps || s_log) s_render_us.push_back((uint32_t)(us < 0 ? 0 : us));
}

static uint64_t fnv(const uint32_t *fb, int stride, int w, int h)
{
    uint64_t x = 0xCBF29CE484222325ULL;
    for (int y = 0; y < h; y++) {
        const uint8_t *p = (const uint8_t *)(fb + (size_t)y * (size_t)stride);
        for (int i = 0; i < w * 4; i++) { x ^= p[i]; x *= 0x100000001B3ULL; }
    }
    return x;
}

static void log_row(uint64_t sync, double t, bool whole, bool gameplay, uint64_t hash)
{
    if (s_scan) {
        fprintf(s_scan, "%llu,%llu,%.1f,%d,%d,%016llX\n", (unsigned long long)s_scan_flip, (unsigned long long)sync, t,
                whole ? 1 : 0, gameplay ? 1 : 0, (unsigned long long)hash);
        if (s_first && !s_trials.empty() && s_trials.back().first == s_scan_flip)
            s_trials.back().second.push_back({sync, hash});
        return;
    }
    s_frames++;
    s_whole += whole;
    if (s_log)
        fprintf(s_log, "%llu,%.1f,%d,%d,%016llX\n", (unsigned long long)sync, t, whole ? 1 : 0, gameplay ? 1 : 0,
                (unsigned long long)hash);
    if (s_first && !s_found) {
        s_main[sync] = {hash, gameplay};
        scan_resolve(sync);
    }
}

void present_init(const char *log_path)
{
    if (log_path) {
        s_log = fopen(log_path, "w");
        if (!s_log) { fprintf(stderr, "[native] cannot write the present log %s\n", log_path); exit(2); }
        setvbuf(s_log, nullptr, _IOFBF, 1 << 20);
        fprintf(s_log, "sync,t_us,whole,gameplay,fb_fnv1a64\n");
    }
}

bool present_logging(void) { return s_log != nullptr; }

/* --branch-plan (main.cpp): a branch's present rows go to its own log, and everything this file
   carries from frame to frame (the interpolation's previous frame and owners, the counters) is put
   back when the branch ends, as a savestate's sections and counters are (none of it is the game's
   state, so it is no section) */
static struct PresentKept {
    std::vector<Mark> marks[NT];
    Tag tags[NT][MAXS], prev_tags[NT][MAXS];
    Snap prev, cur;
    uint64_t frames, whole, sprites, matched, unmatched[5], fallback, extra, pieces;
    std::vector<uint32_t> render_us;
    bool gameplay;
    std::vector<std::pair<uintptr_t, Seen>> seen[NT];   /* s_seen, by instance */
} s_kept;
static uint64_t s_extra, s_pieces;   /* s_pieces: sprites moved by their own step (PIECE_STEP), a count per logic frame */

FILE *present_log_file(FILE *f)   /* swap the log: the one in use is returned */
{
    FILE *old = s_log;
    s_log = f;
    return old;
}

void present_branch_save(void)
{
    PresentKept &k = s_kept;
    for (int t = 0; t < NT; t++) k.marks[t] = s_marks[t];
    memcpy(k.tags, s_tags, sizeof s_tags);
    memcpy(k.prev_tags, s_prev_tags, sizeof s_prev_tags);
    k.prev = s_prev;
    k.cur = s_cur;
    k.frames = s_frames; k.whole = s_whole; k.sprites = s_sprites; k.matched = s_matched;
    memcpy(k.unmatched, s_unmatched, sizeof s_unmatched);
    k.fallback = s_fallback; k.extra = s_extra; k.pieces = s_pieces;
    k.render_us = s_render_us;
    k.gameplay = s_gameplay;
    for (int t = 0; t < NT; t++) {
        k.seen[t].assign(s_seen[t].begin(), s_seen[t].end());
        std::sort(k.seen[t].begin(), k.seen[t].end(),
                  [](const std::pair<uintptr_t, Seen> &a, const std::pair<uintptr_t, Seen> &b) { return a.first < b.first; });
    }
}

void present_branch_restore(void)
{
    const PresentKept &k = s_kept;
    for (int t = 0; t < NT; t++) s_marks[t] = k.marks[t];
    memcpy(s_tags, k.tags, sizeof s_tags);
    memcpy(s_prev_tags, k.prev_tags, sizeof s_prev_tags);
    s_prev = k.prev;
    s_cur = k.cur;
    s_frames = k.frames; s_whole = k.whole; s_sprites = k.sprites; s_matched = k.matched;
    memcpy(s_unmatched, k.unmatched, sizeof s_unmatched);
    s_fallback = k.fallback; s_extra = k.extra; s_pieces = k.pieces;
    s_render_us = k.render_us;
    s_gameplay = k.gameplay;
    for (int t = 0; t < NT; t++) s_seen[t] = std::unordered_map<uintptr_t, Seen>(k.seen[t].begin(), k.seen[t].end());
}

/* --state-at / --state-load (main.cpp "State files"): what present_branch_save keeps, as bytes for
   another process (Mark/Tag owners and anchors point into the arena, at a fixed place there too) */
template <class T> static void put(std::vector<uint8_t> &o, const T &v) { o.insert(o.end(), (const uint8_t *)&v, (const uint8_t *)&v + sizeof v); }
template <class T> static void put_vec(std::vector<uint8_t> &o, const std::vector<T> &v)
{
    put(o, (uint64_t)v.size());
    if (!v.empty()) o.insert(o.end(), (const uint8_t *)v.data(), (const uint8_t *)v.data() + v.size() * sizeof(T));
}
template <class T> static bool get(const uint8_t *&p, const uint8_t *end, T &v)
{
    if ((size_t)(end - p) < sizeof v) return false;
    memcpy(&v, p, sizeof v);
    p += sizeof v;
    return true;
}
template <class T> static bool get_vec(const uint8_t *&p, const uint8_t *end, std::vector<T> &v)
{
    uint64_t n;
    if (!get(p, end, n) || n > (uint64_t)(end - p) / sizeof(T)) return false;
    v.resize((size_t)n);
    if (n) memcpy(v.data(), p, (size_t)n * sizeof(T));
    p += n * sizeof(T);
    return true;
}

void present_state_save(std::vector<uint8_t> &out)
{
    present_branch_save();
    const PresentKept &k = s_kept;
    for (int t = 0; t < NT; t++) put_vec(out, k.marks[t]);
    put(out, k.tags); put(out, k.prev_tags); put(out, k.prev); put(out, k.cur);
    put(out, k.frames); put(out, k.whole); put(out, k.sprites); put(out, k.matched); put(out, k.unmatched);
    put(out, k.fallback); put(out, k.extra); put(out, k.pieces);
    put_vec(out, k.render_us);
    put(out, k.gameplay);
    for (int t = 0; t < NT; t++) put_vec(out, k.seen[t]);
}

bool present_state_load(const uint8_t *p, size_t n)
{
    const uint8_t *end = p + n;
    PresentKept &k = s_kept;
    bool ok = get_vec(p, end, k.marks[0]) && get_vec(p, end, k.marks[1]) && get_vec(p, end, k.marks[2]) && get(p, end, k.tags) && get(p, end, k.prev_tags) && get(p, end, k.prev) &&
              get(p, end, k.cur) && get(p, end, k.frames) && get(p, end, k.whole) && get(p, end, k.sprites) &&
              get(p, end, k.matched) && get(p, end, k.unmatched) && get(p, end, k.fallback) && get(p, end, k.extra) &&
              get(p, end, k.pieces) && get_vec(p, end, k.render_us) && get(p, end, k.gameplay) &&
              get_vec(p, end, k.seen[0]) && get_vec(p, end, k.seen[1]) && get_vec(p, end, k.seen[2]) && p == end;
    if (ok) present_branch_restore();
    return ok;
}

/* A played run's frame-tick V-ints (two players stepping by 2: plat_frame_repeat) are display
   periods of their own: s_extra of them so far, and the next logic frame spans 2 periods when it
   will wait for the tick (g_TwoPlayerStep, $FFC922, unless g_LevelFreeze, $FFC91C). Traced runs
   have none: their clock is the contract's k x T. (s_extra: declared above, with the branch's kept state) */

void present_repeat(void) { s_extra++; }

static int span_now(bool interactive)
{
    return interactive && m8(0xFFC922) && !m8(0xFFC91C) ? 2 : 1;
}

/* plat_frame_end(k): R_k, shown at k x T. Played with high_fps: presented here at its time. */
bool present_whole(uint64_t k, const uint32_t *fb, int stride, int w, int h, bool interactive)
{
    bool display = interactive && host_vsync();   /* pacing=display: at the refresh nearest its time */
    if (!s_log && !g_opt.high_fps && !display) return false;
    bool gameplay = m8(0xFFC942) && !m8(0xFFC7FE);
    log_row(k, (double)k * T_US, true, gameplay, s_log && machine_pixels() ? fnv(fb, stride, w, h) : 0);
    if (display) {
        host_present_refresh(fb, stride, w, h, (double)(k + s_extra) * T_US);
        return true;
    }
    if (interactive && g_opt.high_fps) {
        host_present_at(fb, stride, w, h, (double)(k + s_extra) * T_US);
        return true;
    }
    return false;
}

/* pacing=display, high_fps on: the frames between R_(k-1) at t0 and R_k at t1 are the display's
   refreshes between them (not those nearest t0 and t1, whose pictures are R_(k-1)'s and R_k's), every
   m-th when the display is faster than high_fps (m = its rate over high_fps, rounded up): each shows
   the game interpolated to its own moment. Returns how many (in t). */
static int refresh_times(double t0, double t1, int hz, double *t, int max)
{
    double p = host_refresh_us(), r[64];
    if (p <= 0) return 0;
    int n = host_refreshes(t0 + p * 0.5, t1 - p * 0.5, r, 64), m = std::max(1, (int)std::ceil(1e6 / p / hz - 1e-6)), out = 0;
    for (int i = 0; i < n && out < max; i++) {
        long idx = std::lround((r[i] - t0) / p);   /* refreshes since R_(k-1)'s */
        if (idx % m == 0) t[out++] = r[i];
    }
    return out;
}

/* The sprites an interpolation moves: table t's slot, its step over the span in screen pixels. */
struct Plan { int t, slot; double dx, dy; };
static int s_why[NT][MAXS];   /* why: the `unmatched` counter's index */
static double s_ownx[NT][MAXS], s_owny[NT][MAXS];
static bool s_own[NT][MAXS], s_counted[NT][MAXS];

static bool snaps(int dx, int dy) { return dx > SNAP_SPRITE || dx < -SNAP_SPRITE || dy > SNAP_SPRITE || dy < -SNAP_SPRITE; }

/* Each sprite instance's anchor when it was last drawn, for SEEN_FRAMES frames: an instance drawn
   only every other frame (a player flickering while invulnerable after a respawn, Beast_* and the
   others' blink) has no anchor in R_(k-1), and was left where R_k has it in every in-between frame
   it shows (the audit of 2026-11-18: 70% of the sprites left still). It moves from its last place
   instead, by the part of the step that falls in this span. Up to 9 frames back: a blink that
   slows (a player shown every 5, then up to 9 frames, scenario-continue 11893-11995) too. */
#define SEEN_FRAMES 10

static void seen_update(int t, uint64_t k)
{
    const Tag *tags = s_tags[t];
    for (int s = 0; s < slots(t); s++)
        if (tags[s].spr) s_seen[t][tags[s].spr] = { tags[s].ax, tags[s].ay, s_cur.shift, k };
    for (auto it = s_seen[t].begin(); it != s_seen[t].end();)
        it = it->second.k + SEEN_FRAMES <= k ? s_seen[t].erase(it) : std::next(it);
}

/* on screen and not the HUD: the sprites the interpolation counts */
static bool counted_at(const Snap &n, int t, int s)
{
    const uint8_t *e = ent(n, t, s);
    int y9 = w16(e) & 0x3FF;
    return y9 + 32 > 128 && y9 < 128 + 240 && !hud_attr(w16(e + 4));
}

/* An owner without anchors whose pieces share no step (a timer's particles, each on its own path:
   the ice shards Nightcrawler breaks, TAS 21153-21236; the ninja's ring of puffs, 32 px apart and up
   to 14 px a frame, scenario-char-beast 520-526): when it has as many pieces on screen as in
   R_(k-1), each of the size of the one in its place in the order, each a step of at most PIECE_STEP
   px from it, the pieces are the same ones and each moves by its own step. */
#define PIECE_STEP 16

/* Each sprite's step from R_(k-1): its sprite instance's (the emitter's anchor, the object's place
   on screen), so a frame of animation that rearranges the pieces changes in place, as at 60 Hz,
   instead of each piece sliding from wherever the piece of its index was (ghost limbs beside an
   idling player, part of Cyclops gone in his charged blast: the playtest of 2026-10-21). Pieces
   written by no emitter move with their owner, by the one step that lines up most of its pieces
   with its pieces in R_(k-1) (the same size and tile word; else the same size). None for the HUD, a
   sprite without an owner or instance, a new owner or instance, an owner whose pieces line up
   nowhere, a snap. Steps are of screen places: an anchor is the game's place, so the frames' 16:9
   shifts come off it (widescreen.cpp moves the render camera by up to 40 px near a level's ends; with
   the game's place a standing player shook by the camera's step there, the playtest of 2026-10-31). */
static void plan_table(int t, uint64_t k, std::vector<Plan> &plan)
{
    const Tag *tags = s_tags[t], *prev_tags = s_prev_tags[t];
    int *why = s_why[t];
    double *ownx = s_ownx[t], *owny = s_owny[t];
    bool *own = s_own[t], *counted = s_counted[t];
    /* the prev frame's slots by owner; the instances' anchors are s_seen's */
    std::unordered_map<uintptr_t, std::vector<int>> prev_of;
    for (int s = 0; s < slots(t); s++)
        if (prev_tags[s].owner) prev_of[prev_tags[s].owner].push_back(s);
    const std::unordered_map<uintptr_t, Seen> &seen = s_seen[t];
    std::unordered_map<uintptr_t, std::vector<int>> cur_of;
    int link_mask = !t && g_opt.players > 2 ? 0xFF : 0x7F;
    for (int slot = 0, n = 0; n < slots(t); n++) {
        const uint8_t *c = ent(s_cur, t, slot);
        if (counted_at(s_cur, t, slot)) {
            counted[slot] = true;
            auto pa = tags[slot].spr ? seen.find(tags[slot].spr) : seen.end();
            if (pa != seen.end()) {
                /* the step since it was last drawn, g frames ago: g - 1 of them before this span */
                int g = (int)(k - pa->second.k);
                int dx = (tags[slot].ax - s_cur.shift) - (pa->second.ax - pa->second.shift);
                int dy = tags[slot].ay - pa->second.ay;
                double fx = (double)dx / g, fy = (double)dy / g;
                if (snaps((int)std::lround(fx), (int)std::lround(fy))) why[slot] = 4;
                else { own[slot] = true; ownx[slot] = fx; owny[slot] = fy; }
            } else if (tags[slot].spr)
                why[slot] = 2;
            else if (!tags[slot].owner) {
                /* written outside every owner (a pause screen's text, a loop of its own): the same
                   slot with the same tile and size in R_(k-1) is the same sprite */
                const uint8_t *p = ent(s_prev, t, slot);
                int dx = sx(s_cur, t, slot) - sx(s_prev, t, slot), dy = w16(c) - w16(p);
                if (!prev_tags[slot].owner && p[2] == c[2] && w16(p + 4) == w16(c + 4) && !snaps(dx, dy)) {
                    own[slot] = true; ownx[slot] = dx; owny[slot] = dy;
                    s_fallback++;
                } else
                    why[slot] = 1;
            } else
                cur_of[tags[slot].owner].push_back(slot);
        }
        int link = c[3] & link_mask;
        if (!link || link >= slots(t)) break;
        slot = link;
    }
    for (auto &oc : cur_of) {
        const std::vector<int> &cs = oc.second;
        auto po = prev_of.find(oc.first);
        if (po == prev_of.end()) {
            for (int s : cs) why[s] = 2;
            continue;
        }
        /* votes: each sprite counts once for each step that puts a like piece of R_(k-1) under it */
        int best_votes = 0, bdx = 0, bdy = 0;
        bool any_size = false;
        for (int strict = 1; strict >= 0 && !best_votes; strict--) {
            std::unordered_map<uint32_t, int> votes;
            for (int s : cs) {
                const uint8_t *c = ent(s_cur, t, s);
                std::vector<uint32_t> mine;
                for (int ps : po->second) {
                    const uint8_t *p = ent(s_prev, t, ps);
                    if (p[2] != c[2]) continue;
                    any_size = true;
                    if (strict && w16(p + 4) != w16(c + 4)) continue;
                    int dx = sx(s_cur, t, s) - sx(s_prev, t, ps), dy = w16(c) - w16(p);
                    if (snaps(dx, dy)) continue;
                    uint32_t key = (uint32_t)(dx + 128) << 16 | (uint32_t)(dy + 128);
                    if (std::find(mine.begin(), mine.end(), key) == mine.end()) mine.push_back(key);
                }
                for (uint32_t key : mine) votes[key]++;
            }
            for (auto &v : votes) {   /* the most votes; of equals, the shortest step */
                int dx = (int)(v.first >> 16) - 128, dy = (int)(v.first & 0xFFFF) - 128;
                if (v.second > best_votes
                    || (v.second == best_votes && abs(dx) + abs(dy) < abs(bdx) + abs(bdy))) {
                    best_votes = v.second; bdx = dx; bdy = dy;
                }
            }
        }
        /* a step most pieces disagree with is a coincidence: no step (R_k's places) */
        if (best_votes && best_votes * 3 >= (int)cs.size()) {
            for (int s : cs) { own[s] = true; ownx[s] = bdx; owny[s] = bdy; }
            continue;
        }
        /* each piece its own step (PIECE_STEP): the pieces in slot order, both frames */
        std::vector<int> ps, cso(cs.begin(), cs.end());
        for (int p : po->second)
            if (counted_at(s_prev, t, p)) ps.push_back(p);
        std::sort(cso.begin(), cso.end());
        bool pairs = ps.size() == cso.size();
        for (size_t i = 0; pairs && i < cso.size(); i++) {
            const uint8_t *c = ent(s_cur, t, cso[i]), *p = ent(s_prev, t, ps[i]);
            int dx = sx(s_cur, t, cso[i]) - sx(s_prev, t, ps[i]), dy = w16(c) - w16(p);
            pairs = (p[2] & 0xF) == (c[2] & 0xF) && abs(dx) <= PIECE_STEP && abs(dy) <= PIECE_STEP;
        }
        if (pairs) {
            for (size_t i = 0; i < cso.size(); i++) {
                int s = cso[i];
                own[s] = true;
                ownx[s] = sx(s_cur, t, s) - sx(s_prev, t, ps[i]);
                owny[s] = w16(ent(s_cur, t, s)) - w16(ent(s_prev, t, ps[i]));
            }
            s_pieces += cso.size();
        } else
            for (int s : cs) why[s] = any_size ? 4 : 3;
    }
    for (int slot = 0; slot < slots(t); slot++)
        if (counted[slot] && own[slot]) plan.push_back({ t, slot, ownx[slot], owny[slot] });
}

/* ---- the audit (dev): what the in-between frames leave still --------------------------------- */

/* XMEN2_PRESENT_AUDIT=FILE, for finding motion the interpolation misses. Once per logic frame with
   in-between frames, gameplay frames only:
     U,frame,table,slot,why,attr,size,x,y,owner,owner_w0,spr,spr_def   a sprite left where R_k has it
       (why: the `unmatched` index; owner and spr as 68000 RAM addresses, 0 outside the arena;
       owner_w0 the long at the owner, an actor's class; spr_def the instance's definition)
     A,frame,table,spr,spr_def,owner,owner_w0,adx,ady,pdx,pdy,pieces   a sprite instance whose
       pieces all line up with its pieces in R_(k-1) (same size and tile word) by one step that
       isn't its anchor's: motion by animation frames at a fixed anchor, which moves at 60 Hz
     P,frame,plane,dx,dy,votes,changed   plane A (0) or B (1): on-screen cells rewritten between
       R_(k-1) and R_k of which `votes` hold what the cell (dx, dy) cells away held: a block moved
       by rewriting the nametable, which moves at 60 Hz */
extern "C" uint8_t *xm_arena;
static FILE *s_audit;
static bool s_audit_init;
static uint16_t s_nt_prev[2][128 * 64];
static bool s_nt_valid;

static uint32_t ram_of(uintptr_t p)
{
    uintptr_t a = (uintptr_t)xm_arena;
    return xm_arena && p >= a && p < a + 0x20000u ? 0xFF0000u | (uint32_t)((p - a) & 0xFFFFu) : 0;
}
static uint32_t long_at(uint32_t a) { return a ? (uint32_t)(uint16_t)s16at(a) << 16 | (uint16_t)s16at(a + 2) : 0; }

static bool audit_on(void)
{
    if (!s_audit_init) {
        s_audit_init = true;
        if (const char *p = getenv("XMEN2_PRESENT_AUDIT")) {
            s_audit = fopen(p, "w");
            if (s_audit) setvbuf(s_audit, nullptr, _IOFBF, 1 << 20);
        }
    }
    return s_audit != nullptr;
}

static void audit_planes(uint64_t k, bool log)
{
    const GVDP *v = &g_machine.vdp;
    static const int dims[4] = { 32, 64, 0, 128 };
    int pw = dims[v->reg[16] & 3], ph = dims[(v->reg[16] >> 4) & 3];
    if (!pw || !ph || pw * ph > 128 * 64) { s_nt_valid = false; return; }
    uint32_t base[2] = { (uint32_t)(v->reg[2] & 0x38) << 10, (uint32_t)(v->reg[4] & 7) << 13 };
    uint16_t cur[2][128 * 64];
    for (int pl = 0; pl < 2; pl++)
        for (int i = 0; i < pw * ph; i++) {
            uint32_t a = (base[pl] + 2u * (uint32_t)i) & 0xFFFFu;
            cur[pl][i] = (uint16_t)(v->vram[a] << 8 | v->vram[(a + 1) & 0xFFFFu]);
        }
    if (s_nt_valid && log) {
        int hmode = v->reg[11] & 3, line = 112, he = hmode == 0 ? 0 : hmode == 3 ? line * 4 : (line & ~7) * 4;
        int cw = machine_screen_width() / 8 + 1, ch = 29;
        for (int pl = 0; pl < 2; pl++) {
            int hs = w16(s_cur.hscroll + he + 2 * pl) & 0x3FF, vs = s_cur.vsram[pl] & 0x3FF;
            int x0 = ((-hs) % 1024 + 1024) % 1024 / 8, y0 = vs / 8;
            int votes[9][9] = {}, changed = 0;
            /* not the two cells at each edge: the level's streamer writes the next row or column of
               32 px blocks there as the camera moves (the first audit's "moves", 4 cells apart) */
            for (int cy = 2; cy < ch - 2; cy++)
                for (int cx = 2; cx < cw - 3; cx++) {
                    int x = (x0 + cx) % pw, y = (y0 + cy) % ph, i = y * pw + x;
                    uint16_t c = cur[pl][i];
                    if (c == s_nt_prev[pl][i]) continue;
                    changed++;
                    if (!(c & 0x7FF)) continue;
                    for (int dy = -4; dy <= 4; dy++)
                        for (int dx = -4; dx <= 4; dx++) {
                            if (!dx && !dy) continue;
                            int px = ((x - dx) % pw + pw) % pw, py = ((y - dy) % ph + ph) % ph;
                            if (s_nt_prev[pl][py * pw + px] == c) votes[dy + 4][dx + 4]++;
                        }
                }
            int best = 0, bx = 0, by = 0;
            for (int dy = 0; dy < 9; dy++)
                for (int dx = 0; dx < 9; dx++)
                    if (votes[dy][dx] > best) { best = votes[dy][dx]; bx = dx - 4; by = dy - 4; }
            if (best >= 6)
                fprintf(s_audit, "P,%llu,%d,%d,%d,%d,%d\n", (unsigned long long)k, pl, bx, by, best, changed);
        }
    }
    memcpy(s_nt_prev, cur, sizeof cur);
    s_nt_valid = true;
}

/* A rows: table t's instances in R_k and R_(k-1) */
static void audit_anchors(uint64_t k, int t)
{
    const Tag *tags = s_tags[t], *prev_tags = s_prev_tags[t];
    std::unordered_map<uintptr_t, std::vector<int>> cur_of, prev_of;
    for (int s = 0; s < slots(t); s++) {
        if (tags[s].spr && s_counted[t][s]) cur_of[tags[s].spr].push_back(s);
        if (prev_tags[s].spr) prev_of[prev_tags[s].spr].push_back(s);
    }
    for (auto &oc : cur_of) {
        auto po = prev_of.find(oc.first);
        if (po == prev_of.end()) continue;
        const Tag &tg = tags[oc.second[0]];
        const Tag &pt = prev_tags[po->second[0]];
        int adx = (tg.ax - s_cur.shift) - (pt.ax - s_prev.shift), ady = tg.ay - pt.ay;
        std::unordered_map<uint32_t, int> votes;
        for (int s : oc.second) {
            const uint8_t *c = ent(s_cur, t, s);
            std::vector<uint32_t> mine;
            for (int ps : po->second) {
                const uint8_t *p = ent(s_prev, t, ps);
                if (p[2] != c[2] || w16(p + 4) != w16(c + 4)) continue;
                int dx = sx(s_cur, t, s) - sx(s_prev, t, ps), dy = w16(c) - w16(p);
                if (snaps(dx, dy)) continue;
                uint32_t key = (uint32_t)(dx + 128) << 16 | (uint32_t)(dy + 128);
                if (std::find(mine.begin(), mine.end(), key) == mine.end()) mine.push_back(key);
            }
            for (uint32_t key : mine) votes[key]++;
        }
        int n = (int)oc.second.size();
        for (auto &vt : votes) {
            if (vt.second != n) continue;
            int dx = (int)(vt.first >> 16) - 128, dy = (int)(vt.first & 0xFFFF) - 128;
            if (dx == adx && dy == ady) { votes.clear(); break; }   /* the anchor's step fits: nothing missed */
        }
        int best = 0, bdx = 0, bdy = 0;
        for (auto &vt : votes) {
            int dx = (int)(vt.first >> 16) - 128, dy = (int)(vt.first & 0xFFFF) - 128;
            if (vt.second == n && (!best || abs(dx) + abs(dy) < abs(bdx) + abs(bdy))) { best = n; bdx = dx; bdy = dy; }
        }
        if (!best) continue;
        uint32_t spr = ram_of(oc.first), owner = ram_of(tg.owner);
        fprintf(s_audit, "A,%llu,%d,%06X,%06X,%06X,%06X,%d,%d,%d,%d,%d\n", (unsigned long long)k, t, spr,
                long_at(spr ? spr + 2 : 0), owner, long_at(owner), adx, ady, bdx, bdy, n);
    }
}

/* ---- the frames in between, rendered on worker threads ------------------------------------------
   A full render took 0.45-0.9 ms (2026-11-19, the TAS at 1000 Hz), so at high rates one after
   another on the game's thread, each right before its time, ran late (a reddit question: can it
   run at 1000 Hz?). Each is now a job: a copy of the VDP (and with three or more players of the
   region's sprite tables) with the frame's moves applied, rendered by machine_render_copy into its
   own picture. The live machine is never touched, so they all run at once; the game's thread waits
   for each only when its time comes, and the logic runs after the last (nothing changes the VDP or
   the game's memory while they render). XMEN2_PRESENT_THREADS=N: N workers (0: each on the game's
   thread when its turn comes, the control); default the cores less two, at most 8. */
namespace {
struct Between {
    double a = 0, us = 0;
    std::unique_ptr<GVDP> vdp{ new GVDP };
    std::vector<uint8_t> tables = std::vector<uint8_t>(P34_TABLES_HI - P34_TABLES_LO);
    std::vector<uint32_t> fb;
};
struct Pool {
    std::mutex m;
    std::condition_variable work, done;
    int next = 0, count = 0;      /* the span's jobs [0, count), the next one to take */
    std::vector<char> finished;
};
}
static std::vector<Between> s_between;
static Pool *s_pool;              /* never freed: its workers wait on it until the process ends */
static int s_workers = -1;
static const std::vector<Plan> *s_span_plan;
static bool s_sat_selftest;       /* --sat-selftest (main.cpp) */
void present_sat_selftest(void) { s_sat_selftest = true; }
static uint32_t s_span_hb, s_span_sb;
static int s_span_stride, s_span_rows;

static void render_between(int i)
{
    Between &b = s_between[(size_t)i];
    GVDP *v = b.vdp.get();
    memcpy(v, &g_machine.vdp, sizeof *v);
    const bool region = g_opt.players > 2;
    if (region) memcpy(b.tables.data(), &xm_region[P34_TABLES_LO], b.tables.size());
    const double a = b.a;
    /* the scroll: every line's A and B words, the VSRAM entries */
    for (int i2 = 0; i2 < 0x400; i2 += 2) {
        int p = w16(s_prev.hscroll + i2), c = w16(s_cur.hscroll + i2);
        put16(&v->vram[(uint16_t)(s_span_hb + (uint32_t)i2)], (c & 0xFC00) | lerp_wrap(p & 0x3FF, c & 0x3FF, a, 1024));
    }
    for (int i2 = 0; i2 < GVDP_VSRAM_ENTRIES; i2++)
        v->vsram[i2] = (uint16_t)((s_cur.vsram[i2] & 0xFC00) | lerp_wrap(s_prev.vsram[i2] & 0x3FF, s_cur.vsram[i2] & 0x3FF, a, 1024));
    /* the sprites: back from R_k's position by (1 - a) of their step */
    for (const Plan &pl : *s_span_plan) {
        const uint8_t *c = ent(s_cur, pl.t, pl.slot);
        uint8_t *dst = !pl.t && pl.slot < 80 ? &v->vram[(uint16_t)(s_span_sb + (uint32_t)pl.slot * 8u)]
                                             : &b.tables[rg_off(pl.t) + (uint32_t)pl.slot * 8u - P34_TABLES_LO];   /* players.cpp draws these */
        double back = 1.0 - a;
        put16(dst + 6, w16(c + 6) - (int)(back * pl.dx + (pl.dx >= 0 ? 0.5 : -0.5)));
        put16(dst, w16(c) - (int)(back * pl.dy + (pl.dy >= 0 ? 0.5 : -0.5)));
        /* a VRAM write to the attribute table goes to the VDP's internal copy too, as the port's
           writes do: the renderer reads each sprite's y, size and link from there (the engine's
           sprite attribute cache, pinned 2026-10-24, three days after E4). Without it the sprites
           moved only sideways between frames: their y stayed R_k's until R_k. */
        if (!pl.t && pl.slot < 80 && !s_sat_selftest)   /* --sat-selftest: the bug, for E4's negative control */
            for (uint32_t o : { 0u, 1u, 6u, 7u }) {
                uint16_t addr = (uint16_t)(s_span_sb + (uint32_t)pl.slot * 8u + o);
                gvdp_sat_byte(v, addr, v->vram[addr]);
            }
    }
    size_t need = (size_t)s_span_stride * (size_t)s_span_rows;
    if (b.fb.size() < need) b.fb.resize(need);
    auto r0 = std::chrono::steady_clock::now();
    players_render_tables(region ? b.tables.data() : nullptr);
    machine_render_copy(v, b.fb.data(), s_span_stride, s_span_rows);
    players_render_tables(nullptr);
    b.us = std::chrono::duration<double, std::micro>(std::chrono::steady_clock::now() - r0).count();
}

static void worker(Pool *p)
{
    std::unique_lock<std::mutex> lk(p->m);
    for (;;) {
        p->work.wait(lk, [p] { return p->next < p->count; });
        int i = p->next++;
        lk.unlock();
        render_between(i);
        lk.lock();
        p->finished[(size_t)i] = 1;
        p->done.notify_all();
    }
}

/* the frames of fractions as[1..] (as[0] unused) */
static void between_start(const std::vector<double> &as, const std::vector<Plan> &plan, uint32_t hb, uint32_t sb, int stride)
{
    if (s_workers < 0) {
        const char *e = getenv("XMEN2_PRESENT_THREADS");
        int cores = (int)std::thread::hardware_concurrency();
        s_workers = e && *e ? std::max(0, atoi(e)) : std::min(8, std::max(1, cores - 2));
        if (s_workers) {
            s_pool = new Pool;
            for (int i = 0; i < s_workers; i++) std::thread(worker, s_pool).detach();
        }
    }
    int count = (int)as.size() - 1;
    if ((int)s_between.size() < count) s_between.resize((size_t)count);
    for (int i = 0; i < count; i++) s_between[(size_t)i].a = as[(size_t)i + 1];
    s_span_plan = &plan;
    s_span_hb = hb;
    s_span_sb = sb;
    s_span_stride = stride;
    s_span_rows = machine_screen_height();
    if (!s_pool) return;
    {
        std::lock_guard<std::mutex> lk(s_pool->m);
        s_pool->finished.assign((size_t)count, 0);
        s_pool->next = 0;
        s_pool->count = count;
    }
    s_pool->work.notify_all();
}

/* job i, rendered (by the caller, without workers) */
static const Between &between_wait(int i)
{
    if (!s_pool) render_between(i);
    else {
        std::unique_lock<std::mutex> lk(s_pool->m);
        s_pool->done.wait(lk, [i] { return s_pool->finished[(size_t)i] != 0; });
    }
    return s_between[(size_t)i];
}

/* plat_sync_point, after R_k (k = the frame begun) is rendered and before the frame's logic: the
   frames between R_(k-1) and R_k. */
void present_after_render(uint64_t k, bool interactive, int stride)
{
    if (!g_opt.high_fps && !s_log) return;
    memcpy(s_prev_tags, s_tags, sizeof s_tags);
    for (int t = 0; t < NT; t++) resolve_tags(t);
    snap_now(&s_cur);
    s_gameplay = m8(0xFFC942) && !m8(0xFFC7FE);
    int hz = g_opt.high_fps;
    if (hz && s_prev.valid && k >= 2 && s_prev.reg5 == s_cur.reg5 && s_prev.reg13 == s_cur.reg13) {
        GVDP *v = &g_machine.vdp;
        double t0 = (double)(k - 1 + s_extra) * T_US, t1 = t0 + span_now(interactive) * T_US;
        uint32_t hb = hbase(v), sb = satbase(v);
        std::vector<Plan> plan;
        memset(s_why, 0, sizeof s_why);
        memset(s_own, 0, sizeof s_own);
        memset(s_counted, 0, sizeof s_counted);
        for (int t = 0; t < NT; t++)
            if (s_cur.live[t] && s_prev.live[t]) plan_table(t, k, plan);
        if (s_gameplay && audit_on())
            for (int t = 0; t < NT; t++) {
                if (!(s_cur.live[t] && s_prev.live[t])) continue;
                audit_anchors(k, t);
                for (int slot = 0; slot < slots(t); slot++) {
                    if (!s_counted[t][slot] || s_own[t][slot]) continue;
                    const uint8_t *c = ent(s_cur, t, slot);
                    const Tag &tg = s_tags[t][slot];
                    uint32_t owner = ram_of(tg.owner), spr = ram_of(tg.spr);
                    fprintf(s_audit, "U,%llu,%d,%d,%d,%04X,%X,%d,%d,%06X,%06X,%06X,%06X\n", (unsigned long long)k, t, slot,
                            s_why[t][slot], (unsigned)(uint16_t)w16(c + 4), c[2], sx(s_cur, t, slot), w16(c), owner,
                            long_at(owner), spr, long_at(spr ? spr + 2 : 0));
                }
            }
        const bool draw = machine_pixels();
        /* the span in n even parts, n its H Hz frames rounded: R_k at t1, n - 1 frames before it.
           Not the H Hz clock itself: with R_k at k x T on top of it, 120 showed 180 frames a second,
           bunched (a gap under 2 ms, then one of 15) wherever the clock ran near R_k, and a 120 Hz
           screen dropped one in three of them, unevenly, a stutter every 6 s (a player's report,
           2026-10-30) */
        int n = std::max(1, (int)((t1 - t0) * hz / 1e6 + 0.5));
        /* pacing=display: the refreshes in between instead (refresh_times) */
        const bool display = interactive && host_vsync();
        double rt[64];
        int nr = display ? refresh_times(t0, t1, hz, rt, 64) : 0;
        if (display) n = nr + 1;
        std::vector<double> ts((size_t)n), as((size_t)n);
        for (int j = 1; j < n; j++) {
            ts[(size_t)j] = display ? rt[j - 1] : t0 + j * (t1 - t0) / n;
            as[(size_t)j] = (ts[(size_t)j] - t0) / (t1 - t0);
        }
        /* all of them rendered at once on the workers, each shown at its time as it is ready */
        if (draw && n > 1) between_start(as, plan, hb, sb, stride);
        for (int j = 1; j < n; j++) {
            double t = ts[(size_t)j];
            if (s_gameplay) {
                static int dbg_n;
                if (getenv("XMEN2_PRESENT_DEBUG2") && dbg_n < 30) {
                    int no = 0;
                    for (int slot = 0; slot < MAXS; slot++) no += s_counted[0][slot] && s_why[0][slot] == 1;
                    if (no) { dbg_n++; fprintf(stderr, "[present] frame %llu no_owner %d\n", (unsigned long long)k, no); }
                }
                for (int t = 0; t < NT; t++)
                    for (int slot = 0; slot < MAXS; slot++) {
                        if (!s_counted[t][slot]) continue;
                        s_sprites++;
                        if (!s_own[t][slot]) s_unmatched[s_why[t][slot]]++;
                    }
                s_matched += plan.size();
            }
            if (!draw) {   /* --no-pixels: the frame planned and counted, VRAM untouched, nothing drawn */
                log_row(k, t, false, s_gameplay, 0);
                continue;
            }
            const Between &b = between_wait(j - 1);
            present_note_render(b.us);
            const uint32_t *fb = b.fb.data();
            int w = machine_screen_width(), h = machine_screen_height();
            log_row(k, t, false, s_gameplay, s_log ? fnv(fb, stride, w, h) : 0);
            if (display) host_present_refresh(fb, stride, w, h, t);
            else if (interactive) host_present_at(fb, stride, w, h, t);
            static const char *shots = getenv("XMEN2_PRESENT_SHOTS"), *dir = getenv("XMEN2_DUMP_DIR");
            if (shots && dir) {   /* XMEN2_PRESENT_SHOTS=FROM-TO: the in-between frames of those runner frames */
                unsigned long long lo = 0, hi = 0;
                if (sscanf(shots, "%llu-%llu", &lo, &hi) == 2 && k >= lo && k <= hi) {
                    char path[600];
                    snprintf(path, sizeof path, "%s/p%06llu_%04d.bmp", dir, (unsigned long long)k, (int)(as[(size_t)j] * 1000));
                    host_screenshot(path, fb, stride, w, h);
                }
            }
        }
    }
    if (audit_on()) audit_planes(k, s_gameplay && hz);
    for (int t = 0; t < NT; t++) {
        if (s_cur.live[t]) seen_update(t, k);
        else s_seen[t].clear();
    }
    s_prev = s_cur;
}

void present_reset(void)
{
    s_prev.valid = false;
    for (int t = 0; t < NT; t++) { s_marks[t].clear(); s_seen[t].clear(); }
}

/* rewind.cpp: a played run loaded frame `to`'s state in place of frame `from`'s (a quick load, a
   rewind step). The next frame is shown whole (no frame in between from the other timeline), and the
   present clock, which times frame k at k x T, moves by the jump, so that frame comes one period
   after the last one shown rather than at once (back) or seconds later (forward). */
void present_jump(uint64_t from, uint64_t to)
{
    present_reset();
    host_present_shift(((double)from - (double)to) * T_US);
}

void present_render_us_write(const char *path)
{
    FILE *f = fopen(path, "wb");
    if (!f) { fprintf(stderr, "[native] cannot write XMEN2_RENDER_US %s\n", path); return; }
    if (!s_render_us.empty()) fwrite(s_render_us.data(), sizeof s_render_us[0], s_render_us.size(), f);
    fclose(f);
}

std::string present_counters_json(void)
{
    uint32_t p99 = 0;
    if (!s_render_us.empty()) {
        std::vector<uint32_t> v = s_render_us;
        size_t i = (size_t)((double)(v.size() - 1) * 0.99);
        std::nth_element(v.begin(), v.begin() + (long)i, v.end());
        p99 = v[i];
    }
    if (s_log) fflush(s_log);
    if (s_audit) fflush(s_audit);
    char b[512];
    snprintf(b, sizeof b, "{\"hz\": %d, \"frames\": %llu, \"whole\": %llu, \"sprites\": %llu, \"sprites_matched\": %llu, "
             "\"render_us_p99\": %u, \"unmatched\": {\"no_owner\": %llu, \"new\": %llu, \"size\": %llu, \"snap\": %llu}, "
             "\"fallback_per_logic_frame\": %llu, \"pieces_per_logic_frame\": %llu}",
             g_opt.high_fps ? g_opt.high_fps : 60, (unsigned long long)s_frames,
             (unsigned long long)s_whole, (unsigned long long)s_sprites, (unsigned long long)s_matched, p99,
             (unsigned long long)s_unmatched[1], (unsigned long long)s_unmatched[2], (unsigned long long)s_unmatched[3],
             (unsigned long long)s_unmatched[4], (unsigned long long)s_fallback, (unsigned long long)s_pieces);
    return b;
}
