/* present.cpp - the native runner's display clock and high frame rate (xmen2disasm
 * docs/phases/P6-enhance.md R4, E4, "The contract": --present-log, the `present` counters).
 *
 * The logic stays the 60 Hz frame-synchronous machine. Runner frame k's picture R_k is rendered at
 * sync point k-1, right after the V-int, and shown at k x T (T = 16,688.9 us, one NTSC frame), as
 * the 60 Hz build shows it. With high_fps = H, the frames of an H Hz clock that fall between
 * (k-1) x T and k x T are interpolated from R_(k-1) toward R_k and shown in between: R_k is already
 * rendered when they are made (the logic of runner frame k-1 is done and its pads were read when
 * the 60 Hz build reads them), so nothing waits for anything: no added latency, the in-between
 * frames only show earlier what R_k will show.
 *
 * An interpolated frame is R_k's VDP state rendered with three things moved back toward R_(k-1):
 * the horizontal scroll of every line (the camera and the per-line parallax), the vertical scroll,
 * and every sprite matched to its owner and piece in R_(k-1) (owner: the draw-list node, timer or
 * scene hook whose call wrote it, plat_spr_owner from the game's native branches; piece: its order
 * among its owner's sprites; with the same size). Not moved: the HUD's sprites, unmatched ones, a
 * scroll step over 64 px or a sprite step over 48 px (they snap), the palettes, the planes' tiles.
 * Presentation only: VRAM and VSRAM are put back after each interpolated render.
 */
#include <algorithm>
#include <chrono>
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
struct Mark { uintptr_t owner; int before; };
static std::vector<Mark> s_marks;

extern "C" void plat_spr_owner(const void *owner)
{
    if (!g_opt.high_fps) return;
    int i = s16at(0xFFE21E);
    s_marks.push_back({ (uintptr_t)owner, i == 0x51 ? -1 : i });
}

struct Tag { uintptr_t owner; int piece; };
static Tag s_tags[80], s_prev_tags[80];

static void resolve_tags(void)
{
    for (Tag &t : s_tags) t = { 0, 0 };
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
    for (size_t m = 0; m < s_marks.size(); m++) {
        int from = s_marks[m].before + 1;
        int to = m + 1 < s_marks.size() ? s_marks[m + 1].before : last;
        for (int slot = from, piece = 0; slot <= to && slot < 80; slot++, piece++)
            if (slot >= 0) s_tags[slot] = { s_marks[m].owner, piece };
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

extern "C" void present_note_render(double us)
{
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
    s_frames++;
    s_whole += whole;
    if (s_log)
        fprintf(s_log, "%llu,%.1f,%d,%d,%016llX\n", (unsigned long long)sync, t, whole ? 1 : 0, gameplay ? 1 : 0,
                (unsigned long long)hash);
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

/* plat_frame_end(k): R_k, shown at k x T. Played with high_fps: presented here at its time. */
bool present_whole(uint64_t k, const uint32_t *fb, int stride, int w, int h, bool interactive)
{
    if (!s_log && !g_opt.high_fps) return false;
    bool gameplay = m8(0xFFC942) && !m8(0xFFC7FE);
    log_row(k, (double)k * T_US, true, gameplay, s_log ? fnv(fb, stride, w, h) : 0);
    if (interactive && g_opt.high_fps) {
        host_present_at(fb, stride, w, h, (double)k * T_US);
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
        double t0 = (double)(k - 1) * T_US, t1 = (double)k * T_US, step = 1e6 / hz;
        uint32_t hb = hbase(v), sb = satbase(v);
        /* the prev frame's slots by tag (only tags that occur once) */
        std::unordered_map<uint64_t, int> prev_by_tag;
        std::unordered_map<uint64_t, int> seen;
        for (int s = 0; s < 80; s++)
            if (s_prev_tags[s].owner) {
                uint64_t key = (uint64_t)s_prev_tags[s].owner * 131u + (uint64_t)s_prev_tags[s].piece;
                if (seen[key]++) prev_by_tag.erase(key); else prev_by_tag[key] = s;
            }
        /* each sprite's step from R_(k-1): its own (the same owner's piece, the same size), else its
           owner's (the median of the owner's own steps: a frame of animation that changes the pieces
           still moves with its object); none for the HUD, a sprite without an owner, a snap */
        struct Plan { int slot, dx, dy; };
        std::vector<Plan> plan;
        std::unordered_map<uintptr_t, std::vector<int>> odx, ody;
        int why[80] = { 0 }, ownx[80], owny[80];
        bool own[80] = { false }, counted[80] = { false };
        for (int slot = 0, n = 0; n < 80; n++) {
            const uint8_t *c = s_cur.sat + slot * 8;
            int y9 = w16(c) & 0x3FF;
            if (y9 + 32 > 128 && y9 < 128 + 240 && !hud_attr(w16(c + 4))) {
                counted[slot] = true;
                if (!s_tags[slot].owner) {
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
                } else {
                    auto it = prev_by_tag.find((uint64_t)s_tags[slot].owner * 131u + (uint64_t)s_tags[slot].piece);
                    if (it == prev_by_tag.end()) why[slot] = 2;
                    else {
                        const uint8_t *p = s_prev.sat + it->second * 8;
                        int dx = w16(c + 6) - w16(p + 6), dy = w16(c) - w16(p);
                        if (dx > SNAP_SPRITE || dx < -SNAP_SPRITE || dy > SNAP_SPRITE || dy < -SNAP_SPRITE) why[slot] = 4;
                        else if (p[2] != c[2]) why[slot] = 3;
                        else {
                            own[slot] = true; ownx[slot] = dx; owny[slot] = dy;
                            odx[s_tags[slot].owner].push_back(dx);
                            ody[s_tags[slot].owner].push_back(dy);
                        }
                    }
                }
            }
            int link = c[3] & 0x7F;
            if (!link || link >= 80) break;
            slot = link;
        }
        for (int slot = 0; slot < 80; slot++) {
            if (!counted[slot]) continue;
            if (own[slot]) plan.push_back({ slot, ownx[slot], owny[slot] });
            else if ((why[slot] == 2 || why[slot] == 3) && odx.count(s_tags[slot].owner)) {
                std::vector<int> &xs = odx[s_tags[slot].owner], &ys = ody[s_tags[slot].owner];
                std::nth_element(xs.begin(), xs.begin() + (long)(xs.size() / 2), xs.end());
                std::nth_element(ys.begin(), ys.begin() + (long)(ys.size() / 2), ys.end());
                plan.push_back({ slot, xs[xs.size() / 2], ys[ys.size() / 2] });
            }
        }
        for (double j = (double)(int64_t)(t0 / step) + 1; j * step < t1 - 0.5; j++) {
            double t = j * step, a = (t - t0) / T_US;
            if (t <= t0 + 0.5) continue;
            /* the scroll: every line's A and B words, the VSRAM entries */
            for (int i = 0; i < 0x400; i += 2) {
                int p = w16(s_prev.hscroll + i), c = w16(s_cur.hscroll + i);
                put16(&v->vram[(uint16_t)(hb + (uint32_t)i)], (c & 0xFC00) | lerp_wrap(p & 0x3FF, c & 0x3FF, a, 1024));
            }
            for (int i = 0; i < GVDP_VSRAM_ENTRIES; i++)
                v->vsram[i] = (uint16_t)((s_cur.vsram[i] & 0xFC00) | lerp_wrap(s_prev.vsram[i] & 0x3FF, s_cur.vsram[i] & 0x3FF, a, 1024));
            /* the sprites: back from R_k's position by (1 - a) of their step */
            for (const Plan &pl : plan) {
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
                    if (!own[slot] && !(why[slot] == 2 || why[slot] == 3)) s_unmatched[why[slot]]++;
                }
                s_matched += plan.size();
                for (int slot = 0; slot < 80; slot++)   /* resized or new pieces whose owner moved nowhere */
                    if (counted[slot] && !own[slot] && (why[slot] == 2 || why[slot] == 3) && !odx.count(s_tags[slot].owner))
                        s_unmatched[why[slot]]++;
            }
            auto r0 = std::chrono::steady_clock::now();
            plat_scan_target(ifb);
            machine_render();
            plat_scan_target(nullptr);
            present_note_render(std::chrono::duration<double, std::micro>(std::chrono::steady_clock::now() - r0).count());
            int w = machine_screen_width(), h = machine_screen_height();
            log_row(k, t, false, s_gameplay, s_log ? fnv(ifb, stride, w, h) : 0);
            if (interactive) host_present_at(ifb, stride, w, h, t);
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
