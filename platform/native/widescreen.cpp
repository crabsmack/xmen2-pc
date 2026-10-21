/* widescreen.cpp - the native runner's 16:9 view (xmen2disasm docs/phases/P6-enhance.md R5-R7, E5):
 * which screens go wide, the margin the game's native branches read, the renderer's margins, and
 * the measurements phase.py 6 reads (the `ws` counters: the terrain check, lost sprites, the HUD).
 *
 * The view is 400x224: 40 px beside each side of the original 320 (square pixels, as the host
 * shows them: 224 x 16 / 9 = 398). A screen is wide or 4:3 by the name of the running scene
 * (g_SceneTable[g_SceneIndex], P6-enhance.md "Screen id"); s_screens says which, and
 * game/widescreen_screens.toml says why (E5 compares the two). Everything here reads the game's
 * state through the 68000's view of memory (xm_ram_peek8, the ROM), never the C's variables, so
 * the checks are independent of the code they check.
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

extern "C" {
#include "video/genesis_machine.h"
}
#include "native.h"

#define WS_MARGIN 40            /* px per side: 320 + 2 x 40 = 400 */
static int s_margin = -1;        /* XMEN2_WS_MARGIN=n: another margin, for diagnosing (0: the 4:3 view, checked) */

static int ws_margin(void)
{
    if (s_margin < 0) {
        const char *e = getenv("XMEN2_WS_MARGIN");
        s_margin = e && *e ? atoi(e) : WS_MARGIN;
        if (s_margin < 0 || s_margin > 96) s_margin = WS_MARGIN;
    }
    return s_margin;
}
#define WS_CANVAS (320 + 2 * WS_MARGIN)

/* ---- the game's memory, as the 68000 sees it --------------------------------------------- */

static uint8_t m8(uint32_t a)
{
    a &= 0xFFFFFFu;
    if (a < 0x400000u) return g_rom[a];
    if (a >= 0xFF0000u) return xm_ram_peek8(a & 0xFFFFu);
    return 0;
}
static uint16_t m16(uint32_t a) { return (uint16_t)(m8(a) << 8 | m8(a + 1)); }
static uint32_t m32(uint32_t a) { return (uint32_t)m16(a) << 16 | m16(a + 2); }
static int16_t s16at(uint32_t a) { return (int16_t)m16(a); }

/* ---- screens -------------------------------------------------------------------------------- */

/* The screens that stay 4:3 (R5; each one's reason in game/widescreen_screens.toml): "boot"
   (before the first scene) and the Cerebro screens (briefings, player select, the logos and title,
   credits, failure, the final text). Every other scene is a level and goes wide. */
static bool narrow_screen(const std::string &id)
{
    static const char *allwide = getenv("XMEN2_WS_ALLWIDE");   /* diagnosing: every scene wide */
    if (allwide && *allwide == '1') return id == "boot";
    return id == "boot" || id.compare(0, 14, "CerebroScreen ") == 0;
}

static std::string screen_name(int idx, uint32_t *ptr_out)
{
    uint32_t p = idx >= 0 && idx < 52 ? m32(0xFFF3AEu + 8u * (uint32_t)idx) & 0xFFFFFFu : 0;
    if (ptr_out) *ptr_out = p;
    if (!p || p >= 0x400000u) return "boot";
    std::string s;
    while (p < 0x400000u && g_rom[p] && s.size() < 64) s += (char)g_rom[p++];
    return s;
}

static int s_cache_idx = -1000;
static uint32_t s_cache_ptr;
static std::string s_cache_id;
static int s_cache_wide;
static int s_cerebro;   /* Cerebro_Run is running (plat_ws_cerebro) */

/* Cerebro_Run's native branch: a Cerebro screen begins (1) / ends (0). The player select after a
   death runs under the stage's scene index (PlayerSelect_RunScene restores it before Cerebro_Run),
   so the scene name alone would call it the stage: it is "CerebroScreen PlayerSelect", 4:3. */
extern "C" void plat_ws_cerebro(int on)
{
    s_cerebro = on;
    s_cache_idx = -1000;
}

static void screen_now(void)
{
    int idx = s16at(0xFFCC64);
    uint32_t p = idx >= 0 && idx < 52 ? m32(0xFFF3AEu + 8u * (uint32_t)idx) & 0xFFFFFFu : 0;
    if (idx == s_cache_idx && p == s_cache_ptr) return;
    s_cache_idx = idx;
    s_cache_id = screen_name(idx, &s_cache_ptr);
    if (s_cerebro && s_cache_id.compare(0, 14, "CerebroScreen ") != 0) s_cache_id = "CerebroScreen PlayerSelect";
    s_cache_wide = !narrow_screen(s_cache_id);
    if (getenv("XMEN2_WS_DEBUG"))   /* diagnosing: each change of screen */
        fprintf(stderr, "[ws] frame %llu screen %d %s\n", (unsigned long long)machine_frame_no(), idx, s_cache_id.c_str());
}

/* The game's native branches: the view's margin per side (0 = the original 4:3) */
extern "C" int plat_ws_view(void)
{
    if (!g_opt.widescreen) return 0;
    screen_now();
    return s_cache_wide ? ws_margin() : 0;
}

/* the activation margin (R6: objects activate across the wide view) */
extern "C" int plat_ws_act(void) { return plat_ws_view(); }

/* ---- counters ------------------------------------------------------------------------------- */

static uint64_t s_gameplay_frames, s_terrain_checks, s_terrain_errors, s_sprites_lost, s_sprites_wrapped, s_hud_misplaced;
static std::string s_screens_json;   /* "id": "mode" pairs, in the order first seen */
static std::string s_first_error, s_first_lost;

static void note_screen(void)
{
    screen_now();
    std::string key = "\"" + s_cache_id + "\": ";
    if (s_screens_json.find(key) != std::string::npos) return;
    if (!s_screens_json.empty()) s_screens_json += ", ";
    s_screens_json += key + (s_cache_wide ? "\"wide\"" : "\"4:3\"");
}

/* ---- the terrain check (P6-enhance.md "Definitions") ---------------------------------------- */

static uint16_t vram16(const GVDP *v, uint32_t a) { a &= 0xFFFEu; return (uint16_t)(v->vram[a] << 8 | v->vram[a + 1]); }

/* The name-table word the streamer writes for map tile (tx, ty): its block from the map, the
   block's tile word with the cell's flips and the attribute (Map_PutRowBlockA / _PutColBlockA). A
   row writes two tiles as a long, so a tile number that overflows carries into the tile before
   it (alt); a column writes each word alone (want). */
struct PlaneMap {
    uint32_t map, defs, rows;   /* 68000 addresses */
    int w, h;                   /* blocks */
    uint16_t attr;
};

static void expected(const PlaneMap &pm, int tx, int ty, uint16_t *want, uint16_t *alt)
{
    uint32_t rowoff = (uint32_t)(int32_t)s16at(pm.rows + 2u * (uint32_t)(ty >> 2));
    uint16_t cell = m16(pm.map + 2u * ((uint32_t)(tx >> 2) + rowoff));
    int r = ty & 3, c = tx & 3;
    if (cell & 0x400) r ^= 3;
    if (cell & 0x200) c ^= 3;
    uint32_t t = pm.defs + 2u * ((uint32_t)(cell & 0x1FF) * 16u + (uint32_t)r * 4u);
    uint16_t fl = (uint16_t)((cell & 0x600) << 2 | (pm.attr & 0xE000));
    if (cell & 0x8000) fl ^= 0x8000;
    uint16_t base = pm.attr & 0x7FF;
    *want = (uint16_t)((m16(t + 2u * (uint32_t)c) ^ fl) + base);
    /* the row path: pairs (0,1), (2,3) added as longs */
    int pair = c >> 1;
    uint32_t lw = ((uint32_t)(m16(t + 4u * (uint32_t)pair) ^ fl) << 16 | (uint16_t)(m16(t + 4u * (uint32_t)pair + 2) ^ fl))
                  + ((uint32_t)base << 16 | base);
    *alt = (c & 1) ? (uint16_t)lw : (uint16_t)(lw >> 16);
}

static const bool s_check_all = getenv("XMEN2_WS_CHECK_ALL") && *getenv("XMEN2_WS_CHECK_ALL") == '1';

/* One plane: every cell the view shows (per line: its horizontal scroll, the vertical scroll),
   against the map tile the streamer put there. */
static void check_plane(const GVDP *v, bool plane_a, int margin, int bar_l, int bar_r)
{
    PlaneMap pm;
    int sx, sy, adjx, adjy;
    bool narrow, wrapx, wrapy;
    if (plane_a) {
        pm = { m32(0xFFC77C) & 0xFFFFFFu, m32(0xFFC75A) & 0xFFFFFFu, m32(0xFFC78E) & 0xFFFFFFu,
               s16at(0xFFC792), s16at(0xFFC788), m16(0xFFC796) };
        sx = s16at(0xFFC736); sy = s16at(0xFFC734); adjx = s16at(0xFFC72E); adjy = s16at(0xFFC72C);
        narrow = m8(0xFFEE10) && !margin; wrapx = wrapy = false;
    } else {
        if (!m8(0xFFC79C)) return;   /* no BG map */
        pm = { m32(0xFFC778) & 0xFFFFFFu, m32(0xFFC774) & 0xFFFFFFu, m32(0xFFC78A) & 0xFFFFFFu,
               s16at(0xFFC786), s16at(0xFFC784), m16(0xFFC794) };
        sx = s16at(0xFFC732); sy = s16at(0xFFC730); adjx = s16at(0xFFC72A); adjy = s16at(0xFFC728);
        wrapx = m8(0xFFC74C) != 0; wrapy = m8(0xFFC74A) != 0;
        narrow = !wrapx && m8(0xFFEE12) && !margin;   /* g_BgNarrow */
    }
    if (!pm.map || !pm.defs || !pm.rows || pm.w <= 0 || pm.h <= 0) return;
    const uint8_t *reg = v->reg;
    int wt = (reg[16] & 3) == 3 ? 128 : (reg[16] & 3) == 1 ? 64 : 32;
    int ht = ((reg[16] >> 4) & 3) == 3 ? 128 : ((reg[16] >> 4) & 3) == 1 ? 64 : 32;
    uint32_t base = plane_a ? (uint32_t)(reg[2] & 0x38) << 10 : (uint32_t)(reg[4] & 0x07) << 13;
    uint32_t hbase = (uint32_t)(reg[13] & 0x3F) << 10;
    int hmode = reg[11] & 3;
    bool vcol = (reg[11] & 4) != 0;
    int lines = (reg[1] & 0x08) ? 240 : 224;
    int wx = (reg[17] & 0x1F) * 16, wy = (reg[18] & 0x1F) * 8;
    bool wright = (reg[17] & 0x80) != 0, wdown = (reg[18] & 0x80) != 0;
    static uint8_t seen[128][128];
    memset(seen, 0, sizeof seen);
    for (int y = 0; y < lines; y++) {
        int hidx = hmode == 0 ? 0 : hmode == 2 ? (y & ~7) : y;
        int hs = vram16(v, hbase + (uint32_t)hidx * 4u + (plane_a ? 0u : 2u)) & 0x3FF;
        bool win_v = wdown ? y >= wy : y < wy;
        for (int x = -margin + bar_l; x < 320 + margin - bar_r; x++) {
            if (x == 0 && !s_check_all) x = 320;   /* E5: the wide columns (the 320 between are the original's) */
            if (x >= 320 + margin - bar_r) break;
            if (plane_a && x >= 0 && x < 320 && (win_v || (wright ? x >= wx : x < wx))) continue;   /* the window plane */
            int xc = x < 0 ? 0 : x >= 320 ? 319 : x;
            int vs = vcol ? v->vsram[((xc >> 4) * 2 + (plane_a ? 0 : 1)) % 40] : v->vsram[plane_a ? 0 : 1];
            int px = (x - hs) & (wt * 8 - 1), py = (y + (vs & 0x3FF)) & (ht * 8 - 1);
            seen[py >> 3][px >> 3] = 1;
        }
    }
    int sx0 = sx >> 3, sy0 = sy >> 3, cols = narrow ? 44 : 64;
    for (int r = 0; r < ht && r < 128; r++)
        for (int c = 0; c < wt && c < 128; c++) {
            if (!seen[r][c]) continue;
            int tx = sx0 + ((c - (sx0 + (adjx >> 3))) & 63);
            int ty = sy0 + ((r - (sy0 + (adjy >> 3))) & 31);
            if (wrapx) tx = ((tx % (pm.w * 4)) + pm.w * 4) % (pm.w * 4);
            if (wrapy) ty = ((ty % (pm.h * 4)) + pm.h * 4) % (pm.h * 4);
            s_terrain_checks++;
            uint16_t got = vram16(v, base + (uint32_t)(r * wt + c) * 2u), want = 0, alt = 0;
            /* a cell past the map, or past what a narrow streamer keeps drawn, holds nothing the
               map says: an error (bars cover what the level never shows) */
            bool bad = tx < 0 || ty < 0 || tx >= pm.w * 4 || ty >= pm.h * 4 || (!wrapx && tx - sx0 >= cols);
            if (!bad) expected(pm, tx, ty, &want, &alt);
            if (bad || (got != want && got != alt)) {
                s_terrain_errors++;
                if (s_first_error.empty()) {
                    char b[256];
                    snprintf(b, sizeof b, "plane %c cell (%d,%d) tile (%d,%d) vram %04X map %04X sync %llu",
                             plane_a ? 'A' : 'B', c, r, tx, ty, got, want, (unsigned long long)machine_frame_no());
                    s_first_error = b;
                }
            }
        }
}

/* ---- sprites and the HUD -------------------------------------------------------------------- */

static void check_sprites(int margin)
{
    static const uint16_t p1_attr[] = { 0xA7E8, 0xA7F8, 0xA7FC }, p2_attr[] = { 0xA7D0, 0xA7E0, 0xA7D4 };
    static const int p1_x[] = { 0xA5, 0x8F, 0x97 }, p2_x[] = { 0x17A, 0x1A1, 0x16D, 0x191 };
    bool hud_bad = false;
    int i = 0;
    for (int n = 0; n < 80; n++) {
        uint32_t e = 0xFFE226u + 8u * (uint32_t)i;
        int x = s16at(e + 6);
        uint16_t attr = m16(e + 4);
        /* lost (the contract's definition): the x word lies in the wide view but outside the 9
           bits the VDP reads, so the VDP can't show it there. wrapped: the x word lies outside
           0-511 and the 9 bits would put it inside the wide view (a ghost the renderer hides:
           ws_hide_wrapped) */
        uint8_t size = m8(e + 2);
        int x9 = x & 0x1FF, y9 = s16at(e) & 0x1FF, w = ((size >> 2) + 1) * 8, h = ((size & 3) + 1) * 8;
        if (x != x9 && x + w > 128 - margin && x < 448 + margin) s_sprites_lost++;
        if (x != x9 && x9 + w > 128 - margin && x9 < 448 + margin && y9 + h > 128 && y9 < 128 + 224) {
            s_sprites_wrapped++;
            if (s_first_lost.empty()) {
                char b[160];
                snprintf(b, sizeof b, "slot %d x %d y %d attr %04X size %X frame %llu", i, x, s16at(e), attr,
                         m8(e + 2), (unsigned long long)machine_frame_no());
                s_first_lost = b;
            }
        }
        if ((attr & 0xFF00) == 0xA700) {
            bool p1 = false, p2 = false, ok = false;
            for (uint16_t a : p1_attr) p1 |= attr == a;
            for (uint16_t a : p2_attr) p2 |= attr == a;
            if (p1) for (int h : p1_x) ok |= x == h - margin;
            if (p2) for (int h : p2_x) ok |= x == h + margin;
            if ((p1 || p2) && !ok) hud_bad = true;
        }
        int link = m8(e + 3) & 0x7F;
        if (!link || link >= 80) break;
        i = link;
    }
    if (hud_bad) s_hud_misplaced++;
}

/* ---- bars (R6) ------------------------------------------------------------------------------ */

/* Where the wide view would show what the level never meant to show: past the camera's limits
   (an arena narrower than the wide view: the camera sits in its middle, CAM_MIN_X/CAM_MAX_X) or
   past the FG map's edges, the margin is black. The HUD's sprites stay on top of the bars (their
   rectangles are kept). Presentation only; the terrain check skips the barred columns. */
static int s_bar_l, s_bar_r;
struct Rect { int x0, y0, x1, y1; };
static Rect s_hud[8];
static int s_nhud;

static bool hud_attr(uint16_t attr)
{
    static const uint16_t hud[] = { 0xA7E8, 0xA7F8, 0xA7FC, 0xA7D0, 0xA7E0, 0xA7D4 };
    for (uint16_t a : hud)
        if (attr == a) return true;
    return false;
}

static void bars_now(int m)
{
    s_bar_l = s_bar_r = 0;
    s_nhud = 0;
    if (!m || !m8(0xFFC79E)) return;   /* no FG map: nothing to bar against */
    int cam = s16at(0xFFCD72), lo = s16at(0xFFC770), hi = s16at(0xFFC772);
    int mapw = s16at(0xFFC792) * 32;
    int l = lo - (cam - m), r = (cam + 320 + m) - (hi + 320);
    if (-(cam - m) > l) l = -(cam - m);
    if (mapw > 0 && (cam + 320 + m) - mapw > r) r = (cam + 320 + m) - mapw;
    s_bar_l = l < 0 ? 0 : l > m ? m : l;
    s_bar_r = r < 0 ? 0 : r > m ? m : r;
    if (!s_bar_l && !s_bar_r) return;
    int i = 0;
    for (int n = 0; n < 80; n++) {
        uint32_t e = 0xFFE226u + 8u * (uint32_t)i;
        uint8_t size = m8(e + 2);
        if (hud_attr(m16(e + 4)) && s_nhud < 8) {
            int x = s16at(e + 6) - 128 + m, y = s16at(e) - 128;
            s_hud[s_nhud++] = { x, y, x + ((size >> 2) + 1) * 8, y + ((size & 3) + 1) * 8 };
        }
        int link = m8(e + 3) & 0x7F;
        if (!link || link >= 80) break;
        i = link;
    }
}

/* render_frame (machine.cpp): one output row, in place */
extern "C" void ws_mask_line(int row, uint32_t *argb, int width)
{
    if (!s_bar_l && !s_bar_r) return;
    for (int side = 0; side < 2; side++) {
        int x0 = side ? width - s_bar_r : 0, x1 = side ? width : s_bar_l;
        for (int x = x0; x < x1; x++) {
            bool keep = false;
            for (int k = 0; k < s_nhud && !keep; k++)
                keep = x >= s_hud[k].x0 && x < s_hud[k].x1 && row >= s_hud[k].y0 && row < s_hud[k].y1;
            if (!keep) argb[x] = 0xFF000000u;
        }
    }
}

/* ---- wrapped sprites ------------------------------------------------------------------------ */

/* A sprite whose x word is outside 0-511 is off screen where the game put it; the VDP reads 9 bits,
   which put it 512 px away, and in the wide view that can be inside the picture (in 4:3 it never
   was). For a wide frame those entries of the sprite table in VRAM get y 0 (above the screen) while
   it is rendered, and their y back after. */
static uint16_t s_hidden_at[80], s_hidden_y[80];
static int s_nhidden;

static void hide_wrapped(int m)
{
    s_nhidden = 0;
    if (!m) return;
    GVDP *v = &g_machine.vdp;
    uint32_t sat = (uint32_t)(v->reg[5] & 0x7F) << 9;
    int link = 0;
    for (int n = 0; n < 80; n++) {
        uint16_t e = (uint16_t)(sat + (uint32_t)link * 8u);
        uint16_t x = vram16(v, e + 6u);
        if (x & 0xFE00) {
            s_hidden_at[s_nhidden] = e;
            s_hidden_y[s_nhidden++] = vram16(v, e);
            v->vram[e] = 0;
            v->vram[(uint16_t)(e + 1)] = 0;
        }
        link = v->vram[(uint16_t)(e + 3)] & 0x7F;
        if (!link || link >= 80) break;
    }
}

static void unhide_wrapped(void)
{
    GVDP *v = &g_machine.vdp;
    for (int k = 0; k < s_nhidden; k++) {
        v->vram[s_hidden_at[k]] = (uint8_t)(s_hidden_y[k] >> 8);
        v->vram[(uint16_t)(s_hidden_at[k] + 1)] = (uint8_t)s_hidden_y[k];
    }
    s_nhidden = 0;
}

/* ---- per frame ------------------------------------------------------------------------------ */

/* F10 in a played run (options_toggle_widescreen): the view changes at the next screen */
static int s_pending = -1, s_pending_idx, s_pending_cerebro, s_renderer_wide;

void ws_request_toggle(void)
{
    s_pending = !(s_pending >= 0 ? s_pending : g_opt.widescreen);
    s_pending_idx = s16at(0xFFCC64);
    s_pending_cerebro = s_cerebro;
}

/* Before the frame is rendered: the renderer's margins for this screen, and its bars. */
extern "C" void ws_frame_setup(void)
{
    if (s_pending >= 0 && (s16at(0xFFCC64) != s_pending_idx || s_cerebro != s_pending_cerebro)) {
        options_set_widescreen(s_pending);
        s_pending = -1;
        s_cache_idx = -1000;
    }
    if (!g_opt.widescreen) {
        if (s_renderer_wide) {   /* turned off: the renderer as it was */
            gvdp_set_ws_canvas(0);
            gvdp_set_ws_extra(0);
            gvdp_set_unlimited_sprites(0);
            s_renderer_wide = 0;
            s_bar_l = s_bar_r = 0;
        }
        return;
    }
    s_renderer_wide = 1;
    gvdp_set_ws_canvas(WS_CANVAS);
    gvdp_set_ws_bar_black(1);
    /* a wider line shows more sprites than the VDP's 20 a line; the native machine's status
       never shows the overflow flag, so this is the picture only */
    gvdp_set_unlimited_sprites(1);
    int m = plat_ws_view();
    gvdp_set_ws_extra(m);
    bars_now(m);
    hide_wrapped(m);
}

/* After it: the measurements, on gameplay frames (g_InLevelLoop, no fade). */
extern "C" void ws_frame_check(void)
{
    if (!g_opt.widescreen) return;
    unhide_wrapped();
    note_screen();
    if (!m8(0xFFC942) || m8(0xFFC7FE)) return;
    s_gameplay_frames++;
    int margin = plat_ws_view();
    check_plane(&g_machine.vdp, true, margin, s_bar_l, s_bar_r);
    check_plane(&g_machine.vdp, false, margin, s_bar_l, s_bar_r);
    check_sprites(margin);
}

std::string ws_counters_json(void)
{
    char b[1024];
    snprintf(b, sizeof b, "{\"gameplay_frames\": %llu, \"terrain_checks\": %llu, \"terrain_errors\": %llu, "
             "\"sprites_lost\": %llu, \"sprites_wrapped\": %llu, \"hud_misplaced\": %llu, "
             "\"first_terrain_error\": \"%s\", \"first_sprite_wrapped\": \"%s\", \"screens\": {",
             (unsigned long long)s_gameplay_frames, (unsigned long long)s_terrain_checks,
             (unsigned long long)s_terrain_errors, (unsigned long long)s_sprites_lost,
             (unsigned long long)s_sprites_wrapped, (unsigned long long)s_hud_misplaced,
             s_first_error.c_str(), s_first_lost.c_str());
    return std::string(b) + s_screens_json + "}}";
}
