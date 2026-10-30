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
 * pieces with R_(k-1)'s. Not moved: the HUD's sprites, unmatched ones, a
 * scroll step over 64 px or a sprite step over 48 px (they snap), the palettes, the planes' tiles.
 * Presentation only: VRAM and VSRAM are put back after each interpolated render.
 */
#include <algorithm>
#include <chrono>
#include <map>
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

/* While runner frame k-1's logic builds the sprite list (DMA'd at the next V-int: R_k's), each
   owner's draw is announced with the slot written last before it (g_SprIndex, $FFE21E: $51 = none
   yet); slots are filled in order from 0 (Spr_BeginFrame). */
namespace { struct Mark { uintptr_t owner; int before; uintptr_t spr; int ax, ay; }; }   /* spr: an anchor mark (internal linkage: players.cpp has a Mark too) */
static std::vector<Mark> s_marks;

extern "C" void plat_spr_owner(const void *owner)
{
    players_spr_owner(owner, s16at(0xFFE21E));   /* phase 7: the mp counters' `drawn` */
    if (!g_opt.high_fps) return;
    int i = s16at(0xFFE21E);
    s_marks.push_back({ (uintptr_t)owner, i == 0x51 ? -1 : i, 0, 0, 0 });
}

/* Each sprite emitter (Spr_EmitPre/Stream and their Clip and Linked forms) announces the sprite
   instance whose pieces it writes next and where it draws it: within its owner's slots. */
extern "C" void plat_spr_anchor(const void *spr, long x, long y)
{
    if (!g_opt.high_fps) return;
    int i = s16at(0xFFE21E);
    s_marks.push_back({ 0, i == 0x51 ? -1 : i, (uintptr_t)spr, (int)x, (int)y });
}

struct Tag { uintptr_t owner; int piece; uintptr_t spr; int ax, ay; };
static Tag s_tags[80], s_prev_tags[80];

static void resolve_tags(void)
{
    for (Tag &t : s_tags) t = { 0, 0, 0, 0, 0 };
    int last = s16at(0xFFE21E);
    last = last == 0x51 ? -1 : last;
    static const char *dbg = getenv("XMEN2_PRESENT_DEBUG");
    if (dbg && strtoull(dbg, nullptr, 10) == machine_frame_no()) {
        fprintf(stderr, "[present] frame %llu last %d marks", (unsigned long long)machine_frame_no(), last);
        for (const Mark &m : s_marks) fprintf(stderr, " (%llx,%d)", (unsigned long long)m.owner, m.before);
        fprintf(stderr, "\n");
        for (int s = 0; s <= last && s < 80; s++) {
            uint32_t e = 0xFFE226u + 8u * (uint32_t)s;
            fprintf(stderr, "[present]  slot %d attr %04X x %d y %d size %X\n", s, (unsigned)(uint16_t)s16at(e + 4),
                    s16at(e + 6), s16at(e), m8(e + 2));
        }
    }
    Tag cur = { 0, 0, 0, 0, 0 };
    for (size_t m = 0; m < s_marks.size(); m++) {
        const Mark &k = s_marks[m];
        if (k.spr) { cur.spr = k.spr; cur.ax = k.ax; cur.ay = k.ay; }      /* an anchor: same owner */
        else cur = { k.owner, 0, 0, 0, 0 };                                /* a new owner, no anchor yet */
        int from = k.before + 1;
        int to = m + 1 < s_marks.size() ? s_marks[m + 1].before : last;
        for (int slot = from; slot <= to && slot < 80; slot++, cur.piece++)
            if (slot >= 0) s_tags[slot] = cur;
    }
    s_marks.clear();
}

/* ---- the VDP inputs an interpolation moves --------------------------------------------------- */

struct Snap {
    bool valid;
    uint8_t hscroll[0x400];       /* the horizontal scroll table (224 lines x A, B) */
    uint16_t vsram[GVDP_VSRAM_ENTRIES];
    uint8_t sat[80 * 8];
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
    s->reg11 = v->reg[11];
    s->reg5 = v->reg[5];
    s->reg13 = v->reg[13];
    s->valid = true;
}

static int w16(const uint8_t *p) { return (int16_t)(p[0] << 8 | p[1]); }
static void put16(uint8_t *p, int v) { p[0] = (uint8_t)(v >> 8); p[1] = (uint8_t)v; }

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
    for (int a : hud)
        if ((attr & 0xFFFF) == a) return true;
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
    std::vector<Mark> marks;
    Tag tags[80], prev_tags[80];
    Snap prev, cur;
    uint64_t frames, whole, sprites, matched, unmatched[5], fallback, extra;
    std::vector<uint32_t> render_us;
    bool gameplay;
} s_kept;
static uint64_t s_extra;

FILE *present_log_file(FILE *f)   /* swap the log: the one in use is returned */
{
    FILE *old = s_log;
    s_log = f;
    return old;
}

void present_branch_save(void)
{
    PresentKept &k = s_kept;
    k.marks = s_marks;
    memcpy(k.tags, s_tags, sizeof s_tags);
    memcpy(k.prev_tags, s_prev_tags, sizeof s_prev_tags);
    k.prev = s_prev;
    k.cur = s_cur;
    k.frames = s_frames; k.whole = s_whole; k.sprites = s_sprites; k.matched = s_matched;
    memcpy(k.unmatched, s_unmatched, sizeof s_unmatched);
    k.fallback = s_fallback; k.extra = s_extra;
    k.render_us = s_render_us;
    k.gameplay = s_gameplay;
}

void present_branch_restore(void)
{
    const PresentKept &k = s_kept;
    s_marks = k.marks;
    memcpy(s_tags, k.tags, sizeof s_tags);
    memcpy(s_prev_tags, k.prev_tags, sizeof s_prev_tags);
    s_prev = k.prev;
    s_cur = k.cur;
    s_frames = k.frames; s_whole = k.whole; s_sprites = k.sprites; s_matched = k.matched;
    memcpy(s_unmatched, k.unmatched, sizeof s_unmatched);
    s_fallback = k.fallback; s_extra = k.extra;
    s_render_us = k.render_us;
    s_gameplay = k.gameplay;
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
    put_vec(out, k.marks);
    put(out, k.tags); put(out, k.prev_tags); put(out, k.prev); put(out, k.cur);
    put(out, k.frames); put(out, k.whole); put(out, k.sprites); put(out, k.matched); put(out, k.unmatched);
    put(out, k.fallback); put(out, k.extra);
    put_vec(out, k.render_us);
    put(out, k.gameplay);
}

bool present_state_load(const uint8_t *p, size_t n)
{
    const uint8_t *end = p + n;
    PresentKept &k = s_kept;
    bool ok = get_vec(p, end, k.marks) && get(p, end, k.tags) && get(p, end, k.prev_tags) && get(p, end, k.prev) &&
              get(p, end, k.cur) && get(p, end, k.frames) && get(p, end, k.whole) && get(p, end, k.sprites) &&
              get(p, end, k.matched) && get(p, end, k.unmatched) && get(p, end, k.fallback) && get(p, end, k.extra) &&
              get_vec(p, end, k.render_us) && get(p, end, k.gameplay) && p == end;
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
    if (!s_log && !g_opt.high_fps) return false;
    bool gameplay = m8(0xFFC942) && !m8(0xFFC7FE);
    log_row(k, (double)k * T_US, true, gameplay, s_log && machine_pixels() ? fnv(fb, stride, w, h) : 0);
    if (interactive && g_opt.high_fps) {
        host_present_at(fb, stride, w, h, (double)(k + s_extra) * T_US);
        return true;
    }
    return false;
}

/* plat_sync_point, after R_k (k = the frame begun) is rendered and before the frame's logic: the
   frames between R_(k-1) and R_k. */
void present_after_render(uint64_t k, bool interactive, uint32_t *ifb, int stride)
{
    if (!g_opt.high_fps && !s_log) return;
    memcpy(s_prev_tags, s_tags, sizeof s_tags);
    resolve_tags();
    snap_now(&s_cur);
    s_gameplay = m8(0xFFC942) && !m8(0xFFC7FE);
    int hz = g_opt.high_fps;
    if (hz && s_prev.valid && k >= 2 && s_prev.reg5 == s_cur.reg5 && s_prev.reg13 == s_cur.reg13) {
        GVDP *v = &g_machine.vdp;
        double t0 = (double)(k - 1 + s_extra) * T_US, t1 = t0 + span_now(interactive) * T_US;
        uint32_t hb = hbase(v), sb = satbase(v);
        /* the prev frame's slots by owner, its sprite instances' anchors */
        std::unordered_map<uintptr_t, std::vector<int>> prev_of;
        std::unordered_map<uintptr_t, std::pair<int, int>> prev_anchor;
        for (int s = 0; s < 80; s++) {
            if (s_prev_tags[s].owner) prev_of[s_prev_tags[s].owner].push_back(s);
            if (s_prev_tags[s].spr) prev_anchor[s_prev_tags[s].spr] = { s_prev_tags[s].ax, s_prev_tags[s].ay };
        }
        /* each sprite's step from R_(k-1): its sprite instance's (the emitter's anchor, the object's
           place on screen), so a frame of animation that rearranges the pieces changes in place, as at
           60 Hz, instead of each piece sliding from wherever the piece of its index was (ghost limbs
           beside an idling player, part of Cyclops gone in his charged blast: the playtest of
           2026-10-21). Pieces written by no emitter move with their owner, by the one step that lines
           up most of its pieces with its pieces in R_(k-1) (the same size and tile word; else the same
           size). None for the HUD, a sprite without an owner or instance, a new owner or instance, an
           owner whose pieces line up nowhere, a snap. */
        struct Plan { int slot, dx, dy; };
        std::vector<Plan> plan;
        std::unordered_map<uintptr_t, std::vector<int>> cur_of;
        int why[80] = { 0 }, ownx[80], owny[80];
        bool own[80] = { false }, counted[80] = { false };
        for (int slot = 0, n = 0; n < 80; n++) {
            const uint8_t *c = s_cur.sat + slot * 8;
            int y9 = w16(c) & 0x3FF;
            if (y9 + 32 > 128 && y9 < 128 + 240 && !hud_attr(w16(c + 4))) {
                counted[slot] = true;
                auto pa = s_tags[slot].spr ? prev_anchor.find(s_tags[slot].spr) : prev_anchor.end();
                if (pa != prev_anchor.end()) {
                    int dx = s_tags[slot].ax - pa->second.first, dy = s_tags[slot].ay - pa->second.second;
                    if (dx > SNAP_SPRITE || dx < -SNAP_SPRITE || dy > SNAP_SPRITE || dy < -SNAP_SPRITE) why[slot] = 4;
                    else { own[slot] = true; ownx[slot] = dx; owny[slot] = dy; }
                } else if (s_tags[slot].spr)
                    why[slot] = 2;
                else if (!s_tags[slot].owner) {
                    /* written outside every owner (a pause screen's text, a loop of its own): the same
                       slot with the same tile and size in R_(k-1) is the same sprite */
                    const uint8_t *p = s_prev.sat + slot * 8;
                    int dx = w16(c + 6) - w16(p + 6), dy = w16(c) - w16(p);
                    if (!s_prev_tags[slot].owner && p[2] == c[2] && w16(p + 4) == w16(c + 4)
                        && dx <= SNAP_SPRITE && dx >= -SNAP_SPRITE && dy <= SNAP_SPRITE && dy >= -SNAP_SPRITE) {
                        own[slot] = true; ownx[slot] = dx; owny[slot] = dy;
                        s_fallback++;
                    } else
                        why[slot] = 1;
                } else
                    cur_of[s_tags[slot].owner].push_back(slot);
            }
            int link = c[3] & 0x7F;
            if (!link || link >= 80) break;
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
                    const uint8_t *c = s_cur.sat + s * 8;
                    std::vector<uint32_t> mine;
                    for (int ps : po->second) {
                        const uint8_t *p = s_prev.sat + ps * 8;
                        if (p[2] != c[2]) continue;
                        any_size = true;
                        if (strict && w16(p + 4) != w16(c + 4)) continue;
                        int dx = w16(c + 6) - w16(p + 6), dy = w16(c) - w16(p);
                        if (dx > SNAP_SPRITE || dx < -SNAP_SPRITE || dy > SNAP_SPRITE || dy < -SNAP_SPRITE) continue;
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
            } else
                for (int s : cs) why[s] = any_size ? 4 : 3;
        }
        for (int slot = 0; slot < 80; slot++)
            if (counted[slot] && own[slot]) plan.push_back({ slot, ownx[slot], owny[slot] });
        const bool draw = machine_pixels();
        /* the span in n even parts, n its H Hz frames rounded: R_k at t1, n - 1 frames before it.
           Not the H Hz clock itself: with R_k at k x T on top of it, 120 showed 180 frames a second,
           bunched (a gap under 2 ms, then one of 15) wherever the clock ran near R_k, and a 120 Hz
           screen dropped one in three of them, unevenly, a stutter every 6 s (a player's report,
           2026-10-30) */
        int n = std::max(1, (int)((t1 - t0) * hz / 1e6 + 0.5));
        for (int j = 1; j < n; j++) {
            double t = t0 + j * (t1 - t0) / n, a = (t - t0) / (t1 - t0);
            /* the scroll: every line's A and B words, the VSRAM entries */
            for (int i = 0; i < 0x400 && draw; i += 2) {
                int p = w16(s_prev.hscroll + i), c = w16(s_cur.hscroll + i);
                put16(&v->vram[(uint16_t)(hb + (uint32_t)i)], (c & 0xFC00) | lerp_wrap(p & 0x3FF, c & 0x3FF, a, 1024));
            }
            for (int i = 0; i < GVDP_VSRAM_ENTRIES && draw; i++)
                v->vsram[i] = (uint16_t)((s_cur.vsram[i] & 0xFC00) | lerp_wrap(s_prev.vsram[i] & 0x3FF, s_cur.vsram[i] & 0x3FF, a, 1024));
            /* the sprites: back from R_k's position by (1 - a) of their step */
            for (size_t q = 0; q < plan.size() && draw; q++) {
                const Plan &pl = plan[q];
                const uint8_t *c = s_cur.sat + pl.slot * 8;
                uint8_t *dst = &v->vram[(uint16_t)(sb + (uint32_t)pl.slot * 8u)];
                double back = 1.0 - a;
                put16(dst + 6, w16(c + 6) - (int)(back * pl.dx + (pl.dx >= 0 ? 0.5 : -0.5)));
                put16(dst, w16(c) - (int)(back * pl.dy + (pl.dy >= 0 ? 0.5 : -0.5)));
            }
            if (s_gameplay) {
                static int dbg_n;
                if (getenv("XMEN2_PRESENT_DEBUG2") && dbg_n < 30) {
                    int no = 0;
                    for (int slot = 0; slot < 80; slot++) no += counted[slot] && why[slot] == 1;
                    if (no) { dbg_n++; fprintf(stderr, "[present] frame %llu no_owner %d marks %d\n", (unsigned long long)k, no, (int)s_marks.size()); }
                }
                for (int slot = 0; slot < 80; slot++) {
                    if (!counted[slot]) continue;
                    s_sprites++;
                    if (!own[slot]) s_unmatched[why[slot]]++;
                }
                s_matched += plan.size();
            }
            if (!draw) {   /* --no-pixels: the frame planned and counted, VRAM untouched, nothing drawn */
                log_row(k, t, false, s_gameplay, 0);
                continue;
            }
            auto r0 = std::chrono::steady_clock::now();
            plat_scan_target(ifb);
            machine_render();
            plat_scan_target(nullptr);
            present_note_render(std::chrono::duration<double, std::micro>(std::chrono::steady_clock::now() - r0).count());
            int w = machine_screen_width(), h = machine_screen_height();
            log_row(k, t, false, s_gameplay, s_log ? fnv(ifb, stride, w, h) : 0);
            if (interactive) host_present_at(ifb, stride, w, h, t);
            static const char *shots = getenv("XMEN2_PRESENT_SHOTS"), *dir = getenv("XMEN2_DUMP_DIR");
            if (shots && dir) {   /* XMEN2_PRESENT_SHOTS=FROM-TO: the in-between frames of those runner frames */
                unsigned long long lo = 0, hi = 0;
                if (sscanf(shots, "%llu-%llu", &lo, &hi) == 2 && k >= lo && k <= hi) {
                    char path[600];
                    snprintf(path, sizeof path, "%s/p%06llu_%04d.bmp", dir, (unsigned long long)k, (int)(a * 1000));
                    host_screenshot(path, ifb, stride, w, h);
                }
            }
            /* back to R_k's state */
            for (int i = 0; i < 0x400; i++) v->vram[(uint16_t)(hb + (uint32_t)i)] = s_cur.hscroll[i];
            memcpy(v->vsram, s_cur.vsram, sizeof v->vsram);
            for (int i = 0; i < 80 * 8; i++) v->vram[(uint16_t)(sb + (uint32_t)i)] = s_cur.sat[i];
        }
    }
    s_prev = s_cur;
}

void present_reset(void)
{
    s_prev.valid = false;
    s_marks.clear();
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
    char b[512];
    snprintf(b, sizeof b, "{\"hz\": %d, \"frames\": %llu, \"whole\": %llu, \"sprites\": %llu, \"sprites_matched\": %llu, "
             "\"render_us_p99\": %u, \"unmatched\": {\"no_owner\": %llu, \"new\": %llu, \"size\": %llu, \"snap\": %llu}, "
             "\"fallback_per_logic_frame\": %llu}",
             g_opt.high_fps ? g_opt.high_fps : 60, (unsigned long long)s_frames,
             (unsigned long long)s_whole, (unsigned long long)s_sprites, (unsigned long long)s_matched, p99,
             (unsigned long long)s_unmatched[1], (unsigned long long)s_unmatched[2], (unsigned long long)s_unmatched[3],
             (unsigned long long)s_unmatched[4], (unsigned long long)s_fallback);
    return b;
}
