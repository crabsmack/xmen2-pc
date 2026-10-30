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
#include "state.h"

#define WS_MARGIN 40           /* px per side: 320 + 2 x 40 = 400 */
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

/* the wide view's margin per side on the running screen (0 = the original 4:3) */
static int view_margin(void)
{
    if (!g_opt.widescreen) return 0;
    screen_now();
    return s_cache_wide ? ws_margin() : 0;
}

/* ---- R8's render camera ------------------------------------------------------------------------
   With the view presentation only, the game's camera keeps its 4:3 limits, so at a level's edge the
   400 px centred on it would show past the level. The renderer's window instead starts at
   rx = clamp(camera - m, L, R - 400), [L, R) the x the level shows: the camera's range widened by the
   320-px view, within the FG map. In mid-level rx is camera - m; at an edge the 80 px go to the open
   side. A level narrower than 400 px is centred, with bars past its ends. The shift rx - (camera - m)
   is what the window moves; the streamer keeps a margin more drawn for it (plat_ws_view).
   The FG map's edges are in its own x, g_FgScrollX (Siberia's FG scrolls apart from the camera: its
   16-block map is the 512-px plane, which repeats), and count unless the map is the plane's width:
   that one wraps in it, as the 4:3 view's own scrolling shows (2026-10-22: the camera against the
   map's width barred Siberia's right 40 px on every frame). */
static int s_shift;           /* the shift of the frame being rendered (ws_frame_setup) */
static int s_rx_l, s_rx_r;    /* the level's showable [L, R) at that frame (bars) */

static int shift_max(int m)   /* the streamer keeps at most 92 px a side drawn (Map_StreamColumns' 184) */
{
    int k = 92 - m;
    return k < 0 ? 0 : k < m ? k : m;
}

static int shift_for(int cam, int m, int *L_out, int *R_out)
{
    int lo = s16at(0xFFC770), hi = s16at(0xFFC772), mapw = s16at(0xFFC792) * 32;   /* g_CamMinX, g_CamMaxX, g_FgMapW */
    int L = lo < 0 ? 0 : lo, R = hi + 320;
    int r16 = g_machine.vdp.reg[16] & 3, planew = (r16 == 3 ? 128 : r16 == 1 ? 64 : 32) * 8;
    if (mapw > 0 && mapw != planew) {   /* the map in camera x: its x 0 is where g_FgScrollX puts it now */
        int l0 = cam - s16at(0xFFC73E);
        if (L < l0) L = l0;
        if (R > l0 + mapw) R = l0 + mapw;
    }
    if (L_out) { *L_out = L; *R_out = R; }
    if (!m || g_opt.widescreen_gameplay || !m8(0xFFC79E)) return 0;   /* R6: the camera keeps the view inside */
    int base = cam - m, w = 320 + 2 * m, rx;
    if (R - L >= w) rx = base < L ? L : base > R - w ? R - w : base;
    else rx = (L + R) / 2 - w / 2;
    int s = rx - base, k = shift_max(m);
    return s < -k ? -k : s > k ? k : s;
}

/* The game's native branches (xmen2_options.h) */
extern "C" int plat_ws_view(void)
{
    int m = view_margin();
    if (!m || g_opt.widescreen_gameplay) return m;
    return m + shift_max(m);
}
extern "C" int plat_ws_left(void)
{
    int m = view_margin();
    return m ? m - shift_for(s16at(0xFFCD72), m, nullptr, nullptr) : 0;   /* g_CameraX */
}
extern "C" int plat_ws_right(void)
{
    int m = view_margin();
    return m ? m + shift_for(s16at(0xFFCD72), m, nullptr, nullptr) : 0;
}

/* the gameplay margin (R6: objects activate across the wide view; 0 under R8, presentation only) */
extern "C" int plat_ws_act(void) { return g_opt.widescreen_gameplay ? view_margin() : 0; }

/* the HUD's margin: the view's on each side. The HUD is written at the start of a logic frame, before
   the camera moves, so it can't know the frame's shift; the render camera leaves its sprites instead
   (shift_vram), and they stay at the screen's edges. */
extern "C" int plat_ws_hud(void) { return view_margin(); }

/* ---- counters ------------------------------------------------------------------------------- */

static uint64_t s_gameplay_frames, s_terrain_checks, s_terrain_errors, s_sprites_lost, s_sprites_wrapped, s_hud_misplaced,
                s_bars_unneeded;
static std::string s_screens_json;   /* "id": "mode" pairs, in the order first seen */
static std::string s_first_error, s_first_lost;

/* --branch-plan (main.cpp): the counters' strings as at the branch point, put back when it ends (the
   numbers are state_counters) */
void ws_branch(int save)
{
    static std::string kept[3];
    if (save) { kept[0] = s_screens_json; kept[1] = s_first_error; kept[2] = s_first_lost; }
    else { s_screens_json = kept[0]; s_first_error = kept[1]; s_first_lost = kept[2]; }
}

/* --state-at / --state-load (main.cpp "State files"): the same strings, got (1) / set (0) */
void ws_strings(std::string s[3], int get)
{
    if (get) { s[0] = s_screens_json; s[1] = s_first_error; s[2] = s_first_lost; }
    else { s_screens_json = s[0]; s_first_error = s[1]; s_first_lost = s[2]; }
}

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
static void check_plane(const GVDP *v, bool plane_a, int left, int right, int bar_l, int bar_r)
{
    PlaneMap pm;
    int sx, sy, adjx, adjy;
    bool narrow, wrapx, wrapy;
    if (plane_a) {
        pm = { m32(0xFFC77C) & 0xFFFFFFu, m32(0xFFC75A) & 0xFFFFFFu, m32(0xFFC78E) & 0xFFFFFFu,
               s16at(0xFFC792), s16at(0xFFC788), m16(0xFFC796) };
        sx = s16at(0xFFC736); sy = s16at(0xFFC734); adjx = s16at(0xFFC72E); adjy = s16at(0xFFC72C);
        narrow = m8(0xFFEE10) && !left && !right; wrapx = wrapy = false;
    } else {
        if (!m8(0xFFC79C)) return;   /* no BG map */
        pm = { m32(0xFFC778) & 0xFFFFFFu, m32(0xFFC774) & 0xFFFFFFu, m32(0xFFC78A) & 0xFFFFFFu,
               s16at(0xFFC786), s16at(0xFFC784), m16(0xFFC794) };
        sx = s16at(0xFFC732); sy = s16at(0xFFC730); adjx = s16at(0xFFC72A); adjy = s16at(0xFFC728);
        wrapx = m8(0xFFC74C) != 0; wrapy = m8(0xFFC74A) != 0;
        narrow = !wrapx && m8(0xFFEE12) && !left && !right;   /* g_BgNarrow */
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
        for (int x = -left + bar_l; x < 320 + right - bar_r; x++) {
            if (x == 0 && !s_check_all) x = 320;   /* E5: the wide columns (the 320 between are the original's) */
            if (x >= 320 + right - bar_r) break;
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

static bool hud_attr(uint16_t attr);

/* left, right: the drawn window's margins this frame (R8: they differ at a level's edge) */
static void check_sprites(int left, int right)
{
    static const uint16_t p1_attr[] = { 0xA7E8, 0xA7F8, 0xA7FC }, p2_attr[] = { 0xA7D0, 0xA7E0, 0xA7D4 };
    static const int p1_x[] = { 0xA5, 0x8F, 0x97 }, p2_x[] = { 0x17A, 0x1A1, 0x16D, 0x191 };
    bool hud_bad = false;
    int i = 0, hm = plat_ws_hud();   /* the HUD: the view's margin, the render camera leaves it */
    for (int n = 0; n < 80; n++) {
        uint32_t e = 0xFFE226u + 8u * (uint32_t)i;
        int x = s16at(e + 6);
        uint16_t attr = m16(e + 4);
        /* lost (the contract's definition): the x word lies in the wide view but outside the 9
           bits the VDP reads, so the VDP can't show it there. wrapped: the x word lies outside
           0-511 and the 9 bits would put it inside the wide view (a ghost the renderer hides:
           ws_hide_wrapped) */
        /* R8: the VDP reads the word the render camera shifted (shift_vram), so x is taken in its
           coordinates, where the window is the unshifted one */
        uint8_t size = m8(e + 2);
        int sh = hud_attr(attr) ? 0 : (right - left) / 2;
        x -= sh;
        int x9 = x & 0x1FF, y9 = s16at(e) & 0x1FF, w = ((size >> 2) + 1) * 8, h = ((size & 3) + 1) * 8;
        int vl = 128 - left + sh, vr = 448 + right - sh;
        if (x != x9 && x + w > vl && x < vr) s_sprites_lost++;
        if (x != x9 && x9 + w > vl && x9 < vr && y9 + h > 128 && y9 < 128 + 224) {
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
            if (p1) for (int h : p1_x) ok |= x == h - hm;
            if (p2) for (int h : p2_x) ok |= x == h + hm;
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

/* m: the view's margin; shift: the render camera's (R8; 0 under R6, where the camera itself keeps
   the view inside the level). The window starts at rx = camera - m + shift. */
static void bars_now(int m, int shift)
{
    s_bar_l = s_bar_r = 0;
    s_nhud = 0;
    if (!m || !m8(0xFFC79E)) return;   /* no FG map: nothing to bar against */
    int cam = s16at(0xFFCD72), L, R;
    shift_for(cam, m, &L, &R);
    s_rx_l = L; s_rx_r = R;
    int rx = cam - m + shift, l = L - rx, r = rx + 320 + 2 * m - R;
    s_bar_l = l < 0 ? 0 : l > m - shift ? m - shift : l;   /* never over the game's own 320 px */
    s_bar_r = r < 0 ? 0 : r > m + shift ? m + shift : r;
    if (!s_bar_l && !s_bar_r) return;
    int i = 0;
    for (int n = 0; n < 80; n++) {
        uint32_t e = 0xFFE226u + 8u * (uint32_t)i;
        uint8_t size = m8(e + 2);
        if (hud_attr(m16(e + 4)) && s_nhud < 8) {
            int x = s16at(e + 6) - 128 + m, y = s16at(e) - 128;   /* unshifted (shift_vram) */
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

/* ---- the render camera and wrapped sprites, in VRAM while a frame is rendered ---------------- */

/* R8's shift is applied to what the VDP renders, not to the game: for the frames of one logic frame
   every horizontal scroll word (planes A and B, all 256 lines) shows the window moved by the shift,
   and every sprite of the table in VRAM but the HUD's moves by it; after them, the words go back. The window plane
   is the screen's and stays. present.cpp's frames in between take the shifted words as they are, so
   the shift is interpolated with the camera.
   A sprite whose x word is outside 0-511 (before or after the shift) is off screen where the game put
   it; the VDP reads 9 bits, which put it 512 px away, and in the wide view that can be inside the
   picture (in 4:3 it never was). Those entries get y 0 (above the screen) while it is rendered. */
static uint16_t s_spr_at[80], s_spr_y[80], s_spr_x[80];
static int s_nspr, s_shifted;
static uint8_t s_hscroll[0x400];
static uint32_t s_hscroll_at;

static void put16(GVDP *v, uint32_t a, uint16_t w)
{
    v->vram[(uint16_t)a] = (uint8_t)(w >> 8);
    v->vram[(uint16_t)(a + 1)] = (uint8_t)w;
}

static void shift_vram(int m, int shift)
{
    s_nspr = 0;
    s_shift = shift;
    if (!m) return;
    GVDP *v = &g_machine.vdp;
    uint32_t sat = (uint32_t)(v->reg[5] & 0x7F) << 9;
    int link = 0;
    for (int n = 0; n < 80; n++) {
        uint16_t e = (uint16_t)(sat + (uint32_t)link * 8u);
        uint16_t x = vram16(v, e + 6u);
        /* the game's 16-bit word, shifted: one outside 0-511 before the shift can be inside after */
        int xs = (int16_t)x - (hud_attr(vram16(v, e + 4u)) ? 0 : shift);   /* the HUD stays at the screen's edges */
        if (xs != (int)x || xs < 0 || xs > 0x1FF) {
            s_spr_at[s_nspr] = e;
            s_spr_y[s_nspr] = vram16(v, e);
            s_spr_x[s_nspr++] = x;
            if (xs < 0 || xs > 0x1FF) put16(v, e, 0);
            else put16(v, e + 6u, (uint16_t)xs);
        }
        link = v->vram[(uint16_t)(e + 3)] & 0x7F;
        if (!link || link >= 80) break;
    }
    if (!shift) return;
    s_shifted = 1;
    s_hscroll_at = (uint32_t)(v->reg[13] & 0x3F) << 10;
    for (int i = 0; i < 0x400; i++) s_hscroll[i] = v->vram[(uint16_t)(s_hscroll_at + (uint32_t)i)];
    for (int i = 0; i < 0x400; i += 2) {   /* plane x = screen x - scroll: the window at +shift */
        uint16_t w = vram16(v, s_hscroll_at + (uint32_t)i);
        put16(v, s_hscroll_at + (uint32_t)i, (uint16_t)((w & 0xFC00) | ((w - shift) & 0x3FF)));
    }
}

static void unshift_vram(void)
{
    GVDP *v = &g_machine.vdp;
    for (int k = 0; k < s_nspr; k++) {
        put16(v, s_spr_at[k], s_spr_y[k]);
        put16(v, s_spr_at[k] + 6u, s_spr_x[k]);
    }
    s_nspr = 0;
    if (s_shifted)
        for (int i = 0; i < 0x400; i++) v->vram[(uint16_t)(s_hscroll_at + (uint32_t)i)] = s_hscroll[i];
    s_shifted = 0;
    s_shift = 0;
}

/* players.cpp's sprites past the hardware's 80 (phase 7) are drawn by the host: they move with it */
int ws_render_shift(void) { return s_shift; }

/* ---- per frame ------------------------------------------------------------------------------ */

/* F10 in a played run (options_toggle_widescreen): the view changes at the next screen.
   Online with the view presentation only it is each seat's own (phase 7 R12), and so is the request:
   a rollback's load puts back the seat's latest one (ws_after_load), which stays until the view is
   what it asks, so a replay that crosses the screen change again makes the change again. */
static int s_pending = -1, s_pending_idx, s_pending_cerebro, s_renderer_wide;
static int s_seat_pending = -1, s_seat_idx, s_seat_cerebro;   /* online: the request, outside the snapshot */

static bool view_is_seats(void) { return netplay_enabled() && !g_opt.widescreen_gameplay; }

int ws_request_toggle(void)
{
    s_pending = !(s_pending >= 0 ? s_pending : g_opt.widescreen);
    s_pending_idx = s16at(0xFFCC64);
    s_pending_cerebro = s_cerebro;
    s_seat_pending = s_pending, s_seat_idx = s_pending_idx, s_seat_cerebro = s_pending_cerebro;
    return s_pending;
}

/* Before the frame is rendered: the renderer's margins for this screen, the render camera, its bars. */
extern "C" void ws_frame_setup(void)
{
    if (s_pending >= 0 && (s16at(0xFFCC64) != s_pending_idx || s_cerebro != s_pending_cerebro)) {
        if (!view_is_seats()) {
            options_set_widescreen(s_pending);
            s_pending = -1;
            s_cache_idx = -1000;
        } else if (s_pending != g_opt.widescreen) {
            options_set_widescreen(s_pending);
            s_cache_idx = -1000;
            fprintf(stderr, "[ws] the seat's view %s from frame %llu\n", s_pending ? "16:9" : "4:3",
                    (unsigned long long)machine_frame_no());
        }
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
    int m = view_margin();
    int shift = m && m8(0xFFC79E) ? shift_for(s16at(0xFFCD72), m, nullptr, nullptr) : 0;
    static const int cam_every = getenv("XMEN2_WS_CAM") ? atoi(getenv("XMEN2_WS_CAM")) : 0;   /* diagnosing */
    if (cam_every > 0 && machine_frame_no() % (uint64_t)cam_every == 0)
        fprintf(stderr, "[ws] frame %llu cam %d limits %d..%d targets %d..%d fgmap %dx%d scroll %d adj %d stream %d shift %d bars %d,%d\n",
                (unsigned long long)machine_frame_no(), s16at(0xFFCD72), s16at(0xFFC770), s16at(0xFFC772),
                s16at(0xFFC768), s16at(0xFFC76A), s16at(0xFFC792), s16at(0xFFC788), s16at(0xFFC73E),
                s16at(0xFFC72E), s16at(0xFFC736), shift, s_bar_l, s_bar_r);
    gvdp_set_ws_extra(m);
    bars_now(m, shift);
    shift_vram(m, shift);
}

/* After it: the measurements, on gameplay frames (g_InLevelLoop, no fade). */
extern "C" void ws_frame_check(void)
{
    if (s_nspr || s_shifted) unshift_vram();
    if (!g_opt.widescreen) return;
    note_screen();
    if (!m8(0xFFC942) || m8(0xFFC7FE)) return;
    s_gameplay_frames++;
    int left = plat_ws_left(), right = plat_ws_right();
    if (s_cache_wide) {   /* a 4:3 screen has no wide columns (and its planes aren't a level's map) */
        check_plane(&g_machine.vdp, true, left, right, s_bar_l, s_bar_r);
        check_plane(&g_machine.vdp, false, left, right, s_bar_l, s_bar_r);
        /* R8: a bar where the level shows 400 px or more is one the render camera should have avoided */
        if ((s_bar_l || s_bar_r) && !g_opt.widescreen_gameplay && s_rx_r - s_rx_l >= 320 + left + right)
            s_bars_unneeded++;
    }
    check_sprites(left, right);
}

std::string ws_counters_json(void)
{
    char b[1024];
    snprintf(b, sizeof b, "{\"gameplay_frames\": %llu, \"terrain_checks\": %llu, \"terrain_errors\": %llu, "
             "\"sprites_lost\": %llu, \"sprites_wrapped\": %llu, \"hud_misplaced\": %llu, \"bars_unneeded\": %llu, "
             "\"first_terrain_error\": \"%s\", \"first_sprite_wrapped\": \"%s\", \"screens\": {",
             (unsigned long long)s_gameplay_frames, (unsigned long long)s_terrain_checks,
             (unsigned long long)s_terrain_errors, (unsigned long long)s_sprites_lost,
             (unsigned long long)s_sprites_wrapped, (unsigned long long)s_hud_misplaced, (unsigned long long)s_bars_unneeded,
             s_first_error.c_str(), s_first_lost.c_str());
    return std::string(b) + s_screens_json + "}}";
}

/* ---- savestates (phase 7 R2, state.cpp) -------------------------------------------------------- */

/* What the game's view depends on besides its memory: a Cerebro screen running, the 16:9 view and
   an F10 request for the next screen. The screen cache is derived (rebuilt after a load); the
   counters are the host's. Online with the view each seat's own, the request is the seat's latest. */
static void ws_after_load(void)
{
    s_cache_idx = -1000;
    if (view_is_seats()) s_pending = s_seat_pending, s_pending_idx = s_seat_idx, s_pending_cerebro = s_seat_cerebro;
    if (getenv("XMEN2_WS_DBG")) fprintf(stderr, "[ws] load: frame %llu view %d pending %d idx %d screen %d\n",
                                        (unsigned long long)machine_frame_no(), g_opt.widescreen, s_pending, s_pending_idx, s16at(0xFFCC64));
}

void ws_state_init(void)
{
    STATE_VAR("platform", s_cerebro);
    STATE_VAR("platform", g_opt.widescreen);
    STATE_VAR("platform", s_pending); STATE_VAR("platform", s_pending_idx); STATE_VAR("platform", s_pending_cerebro);
    state_counters(&s_gameplay_frames, sizeof s_gameplay_frames);
    state_counters(&s_terrain_checks, sizeof s_terrain_checks);
    state_counters(&s_terrain_errors, sizeof s_terrain_errors);
    state_counters(&s_sprites_lost, sizeof s_sprites_lost);
    state_counters(&s_sprites_wrapped, sizeof s_sprites_wrapped);
    state_counters(&s_hud_misplaced, sizeof s_hud_misplaced);
    state_counters(&s_bars_unneeded, sizeof s_bars_unneeded);
    state_after_load(ws_after_load);
}

/* the canonical digest's part: not the view or its request, which may be each seat's own (phase 7 R12) */
void ws_digest_fields(std::vector<uint8_t> *out)
{
    out->push_back((uint8_t)s_cerebro);
}
