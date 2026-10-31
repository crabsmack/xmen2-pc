/* players.cpp - four players on the platform side (xmen2disasm docs/phases/P7-multiplayer.md R5-R7,
 * M4): players 3-4's pads, whose code is running (plat_p34_enter), and the `mp` counters the trace
 * reports. The counters read the game's state through the 68000's view of work RAM and players 3-4's
 * region (their objects' flags and positions, the camera, the sprite table), never the C's variables.
 *
 *   gameplay_frames   sync points inside a frame loop (g_InLevelLoop) with no fade (phase 6's)
 *   players_max       the most players in play at once
 *   in_play[4]        per player: gameplay frames with its object present and alive (flags $C000 = $8000)
 *   drawn[4]          of those, frames where it drew a sprite (the draw list's owner marks, phase 6 R4)
 *   off_view[4]       of those, frames with its position outside the camera's view
 *   hud_missing       gameplay frames where a player in play has no HUD panel in the sprite table
 *   hud_missing_at    [frame, player, join]: the first HUD_LIST such (runner frame, player) pairs, join 1
 *                     on the first game frame (g_LevelFrame) of the player's stretch in play (R11 (b):
 *                     the check skips them; a join that makes two players in play turns the game to
 *                     30 Hz, so that frame is shown on two sync points)
 *   hud_panels        [player, first, last]: the stretches of runner frames where a player in play has
 *                     its panel drawn (R11 (b): a players 4 run's missing panel counts only where the
 *                     players 2 run of the same input draws it)
 *   hud_tiles_wrong   of those, frames where a player 1-2's panel tiles in VRAM aren't its panel's art
 *                     (hud_tiles_wrong_at: [frame, player, join] as hud_missing_at); hud_lists_full
 *                     when a list was cut
 *   sprites_dropped   sprite pieces the game's stop dropped (the emitters report them)
 *   snd2              the second sound machine (sound2.cpp)
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>

extern "C" {
#include "video/genesis_machine.h"
}
#include "native.h"
#include "state.h"

extern "C" uint8_t xm_region[0x10000];
extern "C" uint16_t g_pad34[2];

#define P1_OBJ 0xEAA8u   /* g_Player1 */
#define P2_OBJ 0xE9B8u   /* g_Player2 */
#define SPRITE_TABLE 0xE226u
#define VIEW_W 320
#define VIEW_H 224

static int s_p34;   /* the player 3-4 whose code runs (0: none) */
static uint32_t ram_be(uint32_t off, int w);

extern "C" void plat_p34_enter(int k)
{
    s_p34 = k;
    present_p34_enter(k);   /* high_fps: whose sprite table the sprite marks are in */
}

/* ---- players 3-4's tiles (R6: a host-side extension of the VDP model) ------------------------------
 * A player 3-4 runs as player 2 (game/include/xmen2_players34.h), so its sprites use player 2's tile
 * numbers: its character area [g_P2VramTile, + g_PlayerTiles) and the HUD area $FA00-$FCFF. Whatever
 * its code writes there (a DMA: plat_p34_dma; a VRAM port write: players_vram_write) goes to its own
 * tiles instead, and its sprite table (its own, in the region) is drawn from them (players_render). */
#define HUD_LO 0xFA00u
#define HUD_HI 0xFD00u
#define CHR_MAX (0x80u * 32u)
static uint8_t s_chr[2][CHR_MAX];
static uint8_t s_hud[2][HUD_HI - HUD_LO];
static int s_panel_made[2];   /* players 3-4: 1 + the character their panel shows, 0 none yet */
static int s_hud_char[4] = { -1, -1, -1, -1 };   /* each player's character this frame (plat_hud_char) */

static uint32_t chr_lo(void) { return (uint32_t)(uint16_t)ram_be(0xC64C, 2) * 32u; }   /* g_P2VramTile */
static uint32_t chr_hi(void)
{
    uint32_t n = (uint32_t)(uint16_t)ram_be(0xC64A, 2) * 32u;   /* g_PlayerTiles */
    return chr_lo() + (n < CHR_MAX ? n : CHR_MAX);
}

/* the byte of player k's tiles for VRAM address a, or null when a isn't one of its areas */
static uint8_t *bank(int k, uint32_t a)
{
    if (a >= chr_lo() && a < chr_hi()) return &s_chr[k - 3][a - chr_lo()];
    if (a >= HUD_LO && a < HUD_HI) return &s_hud[k - 3][a - HUD_LO];
    return nullptr;
}

extern "C" int plat_p34_dma(unsigned long src, unsigned short dest, unsigned short len)
{
    if (!s_p34 || !bank(s_p34, dest) || !bank(s_p34, (uint32_t)dest + len - 1u)) return 0;
    for (uint32_t i = 0; i < len; i += 2) {
        uint16_t w = machine_peek16((uint32_t)(src + i));
        uint8_t *b = bank(s_p34, (uint32_t)dest + i);
        b[0] = (uint8_t)(w >> 8);
        if (i + 1 < len) b[1] = (uint8_t)w;
    }
    return 1;
}

/* the machine's VRAM port write (a word at a; odd: its bytes swapped, as gvdp_store_vram_word) */
bool players_vram_write(uint16_t a, uint16_t w)
{
    if (!s_p34) return false;
    uint8_t *b = bank(s_p34, a & 0xFFFEu);
    if (!b) return false;
    if (a & 1) { b[0] = (uint8_t)w; b[1] = (uint8_t)(w >> 8); }
    else { b[0] = (uint8_t)(w >> 8); b[1] = (uint8_t)w; }
    return true;
}

/* With three or more players every player's HUD panel is its character's select-screen portrait (the
   user, 2026-10-27; R6a's numbered X discs before): Tbl_CerebroVdp's frame ch (0 Beast .. 6 Magneto, as
   Player_CharIndexOf), a 3x4-tile piece of raw tiles whose top 24x28 are the card, in palette line 1 as
   the HUD is, with a 1 px black (colour 1) outline, centred on the strips beside it (they are a row
   lower than the panel). Into 16 tiles of a 4x4-tile sprite, down the columns. */
#define CEREBRO_SPR 0x0608E2u   /* Tbl_CerebroVdp */
static void portrait_panel(int ch, uint8_t *tiles)
{
    uint8_t px[32][32] = {};
    uint32_t frame = CEREBRO_SPR + 0x24u + 0x14u * (uint32_t)ch;
    uint32_t piece = CEREBRO_SPR + (uint32_t)(g_rom[frame + 0x12] << 8 | g_rom[frame + 0x13]);
    uint32_t art = CEREBRO_SPR + (uint32_t)(g_rom[piece + 6] << 8 | g_rom[piece + 7]);
    const int cx = 4, cy = 3;   /* the card's place: its outline is 26x30 at (3, 2) */
    for (int y = -1; y <= 28; y++)
        for (int x = -1; x <= 24; x++) px[cy + y][cx + x] = 1;
    for (int t = 0; t < 12; t++)
        for (int y = 0; y < 8; y++)
            for (int x = 0; x < 8; x++) {
                int row = (t % 4) * 8 + y, col = (t / 4) * 8 + x;
                if (row >= 28) continue;
                uint8_t b = g_rom[art + (uint32_t)t * 32 + (uint32_t)y * 4 + (uint32_t)x / 2];
                px[cy + row][cx + col] = (uint8_t)(x % 2 ? b & 15 : b >> 4);
            }
    for (int t = 0; t < 16; t++)
        for (int y = 0; y < 8; y++)
            for (int x = 0; x < 8; x += 2)
                tiles[t * 32 + y * 4 + x / 2] =
                    (uint8_t)(px[(t % 4) * 8 + y][(t / 4) * 8 + x] << 4 | px[(t % 4) * 8 + y][(t / 4) * 8 + x + 1]);
}

static bool has_portrait(int ch) { return ch >= 0 && ch <= 6; }

/* P34_HudSprites: player k's (1-4) character this frame, -1 out of play */
extern "C" void plat_hud_char(int k, int ch)
{
    if (k >= 1 && k <= 4) s_hud_char[k - 1] = ch;
}

/* HUD_UpdateHealthBars for player k: its health strip (Art_HUDTiles3: 9 bars, blinking at 2 or fewer)
   and meter (Art_HUDTiles2: charge / 4, at most 8, blinking past it), into its HUD tiles after the panel:
   health then meter, as player 2's sprite (both are right of the middle: P34_HudSprites) */
extern "C" void plat_p34_hud(int k, short health, short charge, int hasMeter, unsigned short frame)
{
    if (k != 3 && k != 4) return;
    int ch = s_hud_char[k - 1];
    if (has_portrait(ch) && s_panel_made[k - 3] != ch + 1) {
        portrait_panel(ch, s_hud[k - 3]);
        s_panel_made[k - 3] = ch + 1;
    }
    unsigned bars = health <= 0 ? 0 : ((unsigned)(health - 1) >> 8) + 1;
    if (bars > 9) bars = 9;
    else if (bars <= 2 && (frame & 0x10)) bars = 0;
    uint32_t hp = 0x200u, mp = 0x280u;
    memcpy(&s_hud[k - 3][hp], &g_rom[0x15BBF4u + bars * 0x80u], 0x80);
    if (hasMeter) {
        unsigned m = (unsigned)(uint16_t)charge >> 2;
        if (m > 8) m = 8;
        if (m == 8 && (frame & 8)) m++;
        memcpy(&s_hud[k - 3][mp], &g_rom[0x15B6F4u + m * 0x80u], 0x80);
    }
}

/* ---- drawing players 3-4's sprite tables (the VDP model's host sprites, after its own) ------------- */

static uint32_t table_off(int k) { return 0x0400u + 0x300u * (uint32_t)(k - 3); }
static uint32_t px_off(int k) { return 0x0200u + 40u * (uint32_t)(k - 3); }   /* g_PX[k - 3] (PlayerX: 40 bytes) */
static int spr_last(int k) { return (int16_t)(xm_region[px_off(k) + 36] << 8 | xm_region[px_off(k) + 37]); }

static uint32_t reg_be(uint32_t off, int w)
{
    uint32_t v = 0;
    for (int i = 0; i < w; i++) v = v << 8 | xm_region[off + (uint32_t)i];
    return v;
}

/* the entries of player k's table this frame, in link order (empty: nothing written since its begin) */
static int table_entries(int k, int *out)
{
    if (spr_last(k) == 0x51) return 0;
    int n = 0, idx = 0;
    while (n < 80) {
        out[n++] = idx;
        int link = (int)reg_be(table_off(k) + 8u * (uint32_t)idx + 3, 1) & 0x7F;
        if (!link || link >= 80) break;
        idx = link;
    }
    return n;
}

/* one hardware sprite (8 bytes, big-endian, as the SAT holds it) on this output row, behind every
   sprite already there; k: a player 3-4 whose own tiles stand for player 2's areas, else 0 (VRAM).
   A player 3-4's HUD piece (its panel and strips) goes in front of them instead, as the HUD is in
   front of every sprite of the main table (Level_FrameLoop writes it first). */
static void draw_entry(const uint8_t *e, int k, const GVDP *v, const GVDPSpriteLayer *layer, uint32_t lo, uint32_t hi)
{
    int y = (int)((e[0] << 8 | e[1]) & 0x1FF) - 128, size = e[2];
    uint16_t attr = (uint16_t)(e[4] << 8 | e[5]);
    int x = (int)((e[6] << 8 | e[7]) & 0x1FF) - 128;
    bool hud = k && (attr & 0x7FF) >= 0x7D0 && (attr & 0x7FF) < 0x7E8;
    if (!hud)   /* R8: a panel stays at the screen's edge */
        x -= ws_render_shift();
    int w = ((size >> 2) & 3) + 1, h = (size & 3) + 1;
    int row = layer->line - y;
    if (row < 0 || row >= h * 8) return;
    if (attr & 0x1000) row = h * 8 - 1 - row;   /* V flip */
    int pal = (attr >> 13) & 3, tile0 = attr & 0x7FF, pri = (attr & 0x8000) != 0;
    for (int col = 0; col < w * 8; col++) {
        int sx = layer->offset + x + col;
        if (sx < 0 || sx >= layer->total || (layer->opaque[sx] && !hud)) continue;
        int c = attr & 0x800 ? w * 8 - 1 - col : col;   /* H flip */
        uint32_t a = (uint32_t)(tile0 + (c >> 3) * h + (row >> 3)) * 32u + (uint32_t)(row & 7) * 4u + (uint32_t)(c & 7) / 2u;
        a &= 0xFFFFu;
        uint8_t byte;
        if (k && a >= lo && a < hi) byte = s_chr[k - 3][a - lo];
        else if (k && a >= HUD_LO && a < HUD_HI) byte = s_hud[k - 3][a - HUD_LO];
        else byte = v->vram[a];
        int nib = c & 1 ? byte & 15 : byte >> 4;
        if (!nib) continue;
        layer->index[sx] = (uint8_t)(pal * 16 + nib);
        layer->opaque[sx] = 1;
        layer->high[sx] = (uint8_t)pri;
    }
}

/* Players 3-4's tables are filled only by Level_FrameLoop (P34_SprBeginFrame/EndFrame) and kept as they
   are outside it: on the player select after a stage they showed player 3's last HUD panel and pieces
   of its character from tiles holding other art by now (the playtest of 2026-10-31). They are drawn
   while a level loop runs ($FFC942, g_InLevelLoop) that isn't the select (g_P34SelectOn), which sets
   that flag too. */
bool players_p34_tables_shown(void)
{
    return g_opt.players > 2 && ram_be(0xC942, 1) && !reg_be(0x0284, 2);
}

/* With three or more players the main sprite table is in the region with 160 slots (SPR_STOP): the
   hardware shows the first 80 (VRAM's attribute table, its chain ended there: players_frame_setup),
   this the rest, then players 3-4's own tables, each behind what is already drawn */
#define MAIN_TABLE 0x1000u
static void host_sprites(void *user, const GVDP *v, const GVDPSpriteLayer *layer)
{
    (void)user;
    uint32_t lo = chr_lo(), hi = chr_hi();
    int idx = 0;
    for (int n = 0; n < 160 && g_opt.players > 2; n++) {   /* the main table past the hardware's 80 */
        const uint8_t *e = &xm_region[MAIN_TABLE + 8u * (uint32_t)idx];
        if (idx >= 80) draw_entry(e, 0, v, layer, lo, hi);
        int link = e[3];
        if (!link || link >= 160) break;
        idx = link;
    }
    bool shown = players_p34_tables_shown();
    int ents[80];
    for (int k = 3; k <= g_opt.players && k <= 4 && shown; k++) {
        int n = table_entries(k, ents);
        for (int i = 0; i < n; i++) draw_entry(&xm_region[table_off(k) + 8u * (uint32_t)ents[i]], k, v, layer, lo, hi);
    }
}

/* Before a frame is rendered (machine.cpp): VRAM's attribute table, which the game's DMA filled with
   the main table's first 80 slots, ends its chain there (a link past 80 would make the VDP read past
   the table); and players 1-2's panels in the table show their portraits (portrait_panel) from their
   panel tiles, the X discs there for the render only. After it, VRAM as it was: the game never sees
   the portraits. And no per-line sprite limits (R6). */
static uint16_t s_sat_patched[80];
static uint8_t s_link_was[80];
static int s_npatched;
static const uint16_t k_panel_tile[2] = { 0x7E8, 0x7D0 };   /* players 1-2's panel tiles (Level_FrameLoop) */
static uint8_t s_panel_was[2][16 * 32];
static int s_panel_swapped[2];
void players_frame_setup(void)
{
    if (g_opt.players <= 2) return;
    gvdp_set_unlimited_sprites(1);
    GVDP *v = &g_machine.vdp;
    s_npatched = 0;
    bool panel[2] = {};
    for (int i = 0; i < 80; i++) {
        uint16_t a = (uint16_t)(0xF000u + 8u * (uint32_t)i + 3u);
        if ((v->vram[a] & 0x7F) >= 80) {
            s_sat_patched[s_npatched] = a;
            s_link_was[s_npatched++] = v->vram[a];
            v->vram[a] = 0;
        }
        uint16_t attr = (uint16_t)(v->vram[a + 1] << 8 | v->vram[a + 2]);
        /* a panel: 4x4 tiles (the "press start" in player 2's tiles is 4x1) on the HUD's row (Level_FrameLoop's
           y $132): player 3's cursor box on the player select is a 4x4 of tile $7D0 too (BOX_TILE + 16), and
           showed player 2's portrait in its colours (the playtest of 2026-10-31) */
        bool hud_row = (v->vram[a - 3] << 8 | v->vram[a - 2]) == 0x132;
        for (int p = 0; p < 2; p++)
            if (hud_row && (v->vram[a - 1] & 0xF) == 0xF && (attr & 0x7FF) == k_panel_tile[p]) panel[p] = true;
    }
    for (int p = 0; p < 2; p++) {
        if (!panel[p] || !has_portrait(s_hud_char[p])) continue;
        uint8_t *t = &v->vram[k_panel_tile[p] * 32u];
        memcpy(s_panel_was[p], t, sizeof s_panel_was[p]);
        portrait_panel(s_hud_char[p], t);
        s_panel_swapped[p] = 1;
    }
}

void players_frame_restore(void)
{
    GVDP *v = &g_machine.vdp;
    for (int i = 0; i < s_npatched; i++) v->vram[s_sat_patched[i]] = s_link_was[i];
    s_npatched = 0;
    for (int p = 0; p < 2; p++)
        if (s_panel_swapped[p]) {
            memcpy(&v->vram[k_panel_tile[p] * 32u], s_panel_was[p], sizeof s_panel_was[p]);
            s_panel_swapped[p] = 0;
        }
}

extern "C" void plat_spr_dropped(int n) { players_spr_dropped(n); }

void players_video_init(void)
{
    if (g_opt.players > 2) gvdp_set_host_sprites(host_sprites, nullptr);
    state_section_range("p34video", s_chr, sizeof s_chr);
    state_section_range("p34video", s_hud, sizeof s_hud);
    state_section_range("p34video", s_panel_made, sizeof s_panel_made);
    state_section_range("p34video", s_hud_char, sizeof s_hud_char);
}
/* players 3-4's pads in the game's format, as Pad_ReadPort reads a 6-button pad (players 1-2's are
   6-button too): Up Down Left Right B C A Start in the low byte, Z Y X Mode in bits 8-11, $4000 */
extern "C" unsigned short plat_pad34(int k)
{
    if (k != 3 && k != 4) return 0;
    uint16_t g = g_pad34[k - 3];
    return (unsigned short)(0x4000 | (g & 0xFF) | ((g & GPAD_Z) ? 0x100 : 0) | ((g & GPAD_Y) ? 0x200 : 0) |
                            ((g & GPAD_X) ? 0x400 : 0) | ((g & GPAD_MODE) ? 0x800 : 0));
}
int players_p34_running(void) { return s_p34; }

/* ---- the game's state, read as the 68000 sees it ------------------------------------------------- */

static uint32_t ram_be(uint32_t off, int w)
{
    uint32_t v = 0;
    for (int i = 0; i < w; i++) v = v << 8 | xm_ram_peek8((off + (uint32_t)i) & 0xFFFFu);
    return v;
}

static uint32_t obj_be(int p, uint32_t field, int w)   /* player p's (1-4) object */
{
    if (p <= 2) return ram_be((p == 1 ? P1_OBJ : P2_OBJ) + field, w);
    uint32_t v = 0, base = p == 3 ? 0x0000u : 0x0100u;
    for (int i = 0; i < w; i++) v = v << 8 | xm_region[base + field + (uint32_t)i];
    return v;
}

static uint32_t obj_addr(int p)   /* the 68000 value of player p's object (the owner marks') */
{
    return p == 1 ? 0xFFFF0000u + P1_OBJ : p == 2 ? 0xFFFF0000u + P2_OBJ : 0x00FE0000u + (p == 3 ? 0u : 0x100u);
}

static bool alive(int p) { return (obj_be(p, 4, 2) & 0xC000u) == 0x8000u; }

/* ---- the owner marks (present.cpp's plat_spr_owner passes them on): who drew which sprites ------ */

extern "C" uint8_t *xm_arena, *xm_arena_ffff;

/* a host pointer's 68000 address in work RAM (either view: $FFxxxx) or players 3-4's region, else 0 */
static uint32_t addr68(const void *p)
{
    uintptr_t u = (uintptr_t)p;
    if (u - (uintptr_t)xm_arena < 0x10000u) return 0xFFFF0000u + (uint32_t)(u - (uintptr_t)xm_arena);
    if (u - (uintptr_t)xm_arena_ffff < 0x10000u) return 0xFFFF0000u + (uint32_t)(u - (uintptr_t)xm_arena_ffff);
    if (u - (uintptr_t)xm_region < 0x10000u) return 0x00FE0000u + (uint32_t)(u - (uintptr_t)xm_region);
    return 0;
}

/* internal linkage: present.cpp has a Mark of its own, and the two vectors' out-of-line members
   (one per name: the linker keeps either file's) grew this one with the other's element size */
namespace { struct Mark { uint32_t owner; int idx; }; }
static std::vector<Mark> s_marks;

void players_spr_owner(const void *owner, int idx) { s_marks.push_back({ addr68(owner), idx }); }

/* ---- counters ------------------------------------------------------------------------------------- */

enum { HUD_LIST = 256 };

static struct {
    uint64_t gameplay, in_play[4], drawn[4], off_view[4], hud_missing, dropped;
    uint64_t players_max;
    uint32_t miss[HUD_LIST][3], n_miss;      /* hud_missing_at */
    uint32_t panel[HUD_LIST][3], n_panel;    /* hud_panels */
    uint64_t tiles_wrong;
    uint32_t wrong[HUD_LIST][3], n_wrong;    /* hud_tiles_wrong_at */
    uint32_t open_panel[4];                  /* each player's stretch in panel[] still growing: index + 1, 0 none */
    uint32_t in_play_prev, lists_full;       /* the players in play on the sync point before (bits 0-3) */
    uint16_t join_frame[4];                  /* g_LevelFrame on the first sync point of each one's stretch in play */
} C;

void players_spr_dropped(int n) { if (n > 0) C.dropped += (uint64_t)n; }

/* a HUD panel of player p in the frame's sprite table (the game's own tiles: Level_FrameLoop) */
static bool hud_panel(int p)
{
    int idx = 0, guard = 0;
    uint16_t want = p == 1 ? 0xA7E8 : 0xA7D0;
    if (g_opt.players > 2 && p <= 2) {   /* the main table is in the region (160 slots) */
        for (; guard < 162; guard++) {
            const uint8_t *e = &xm_region[0x1000u + 8u * (uint32_t)idx];
            if ((uint16_t)(e[4] << 8 | e[5]) == want) return true;
            if (!e[3] || e[3] >= 160) break;
            idx = e[3];
        }
        return false;
    }
    if (p > 2) {   /* players 3-4's panels are in their own tables (P34_HudSprites) */
        int ents[80], n = table_entries(p, ents);
        for (int i = 0; i < n; i++)
            if ((uint16_t)reg_be(table_off(p) + 8u * (uint32_t)ents[i] + 4, 2) == want) return true;
        return false;
    }
    for (; guard < 82; guard++) {
        uint32_t e = SPRITE_TABLE + 8u * (uint32_t)idx;
        if ((uint16_t)ram_be(e + 4, 2) == want) return true;
        int link = (int)(ram_be(e + 3, 1) & 0x7F);
        if (link == 0 || link >= 80) break;
        idx = link;
    }
    return false;
}

/* player p's (1-2) panel tiles in VRAM are its panel (HUD_Redraw: Art_HUDTiles, player 2's $200 bytes
   on, at $FD00 / $FA00), as the game wrote them (players_frame_setup's portraits are the render's only) */
#define ART_HUD_TILES 0x15B2F4u
static bool hud_tiles(int p)
{
    uint32_t a = p == 1 ? 0xFD00u : 0xFA00u;
    return !memcmp(&g_machine.vdp.vram[a], &g_rom[ART_HUD_TILES + (p == 1 ? 0u : 0x200u)], 0x200);
}

/* XMEN2_MP_LOG=1: each change of the scene, of who is in play and of the level loop, by runner frame
   (what tools/mp_inputs.py reads to place a stage input's presses) */
static void mp_log(void)
{
    static const bool on = getenv("XMEN2_MP_LOG") && *getenv("XMEN2_MP_LOG") == '1';
    if (!on) return;
    static int last_scene = -2, last_mask = -1, last_loop = -1;
    int scene = (int16_t)ram_be(0xCC64, 2), loop = xm_ram_peek8(0xC942), mask = 0;
    for (int p = 1; p <= 4; p++) mask |= alive(p) ? 1 << (p - 1) : 0;
    if (scene != last_scene || mask != last_mask || loop != last_loop)
        fprintf(stderr, "[mp] frame %llu scene %d loop %d alive %x\n", (unsigned long long)machine_frame_no(), scene, loop, mask);
    last_scene = scene;
    last_mask = mask;
    last_loop = loop;
}

void players_sync_row(void)
{
    mp_log();
    bool gameplay = xm_ram_peek8(0xC942) && !xm_ram_peek8(0xC7FE);
    int end_idx = (int)(int16_t)ram_be(0xE21E, 2);
    if (gameplay) {
        C.gameplay++;
        int cam_x = (int16_t)ram_be(0xCD72, 2), cam_y = (int16_t)ram_be(0xCD70, 2);
        /* XMEN2_MP_POS=n (diagnosing off_view): the camera and the players' positions every n frames */
        static const int pos_every = getenv("XMEN2_MP_POS") ? atoi(getenv("XMEN2_MP_POS")) : 0;
        if (pos_every && machine_frame_no() % pos_every == 0) {
            fprintf(stderr, "[pos] frame %llu cam %d,%d", (unsigned long long)machine_frame_no(), cam_x, cam_y);
            for (int p = 1; p <= 4; p++)
                fprintf(stderr, " p%d %d,%d h%d", p, (int)((int32_t)obj_be(p, 8, 4) >> 8), (int)((int32_t)obj_be(p, 12, 4) >> 8), alive(p));
            fprintf(stderr, "\n");
        }
        int ml = plat_ws_left(), mr = plat_ws_right();   /* the drawn window (R8: its sides differ at an edge) */
        uint64_t n = 0;
        bool hud_ok = true, hud_on = xm_ram_peek8(0xC94C) != 0;
        uint32_t frame = (uint32_t)machine_frame_no(), in_play = 0;
        uint16_t level_frame = (uint16_t)ram_be(0xC7DC, 2);   /* g_LevelFrame: one per pass of Level_FrameLoop */
        for (int p = 1; p <= 4; p++) {
            if (p > 2 && g_opt.players < p) continue;
            if (!alive(p)) continue;
            n++;
            C.in_play[p - 1]++;
            in_play |= 1u << (p - 1);
            if (!(C.in_play_prev >> (p - 1) & 1)) C.join_frame[p - 1] = level_frame;
            bool drew = false;
            if (p <= 2) {
                for (size_t j = 0; j < s_marks.size(); j++)
                    if (s_marks[j].owner == obj_addr(p)) {
                        int next = j + 1 < s_marks.size() ? s_marks[j + 1].idx : end_idx;
                        if (next != s_marks[j].idx) drew = true;
                    }
            } else {   /* players 3-4 draw into their own tables: a sprite there that isn't their HUD's */
                int ents[80], ne = table_entries(p, ents);
                for (int i = 0; i < ne; i++) {
                    int tile = (int)(reg_be(table_off(p) + 8u * (uint32_t)ents[i] + 4, 2) & 0x7FF);
                    if (tile < 0x7D0 || tile >= 0x7E8) drew = true;
                }
            }
            if (drew) C.drawn[p - 1]++;
            static const char *dbg = getenv("XMEN2_MP_DEBUG");   /* diagnosing: player N's undrawn frames */
            if (dbg && !drew && atoi(dbg) == p && C.in_play[p - 1] - C.drawn[p - 1] < 6) {
                fprintf(stderr, "[mp] frame %llu: player %d (%08x) alive, not drawn; marks:", (unsigned long long)machine_frame_no(),
                        p, (unsigned)obj_addr(p));
                for (const Mark &mk : s_marks) fprintf(stderr, " %08x@%d", (unsigned)mk.owner, mk.idx);
                fprintf(stderr, " end@%d\n", end_idx);
            }
            int x = (int)((int32_t)obj_be(p, 8, 4) >> 8), y = (int)((int32_t)obj_be(p, 12, 4) >> 8);
            if (x < cam_x - ml || x >= cam_x + VIEW_W + mr || y < cam_y || y >= cam_y + VIEW_H) C.off_view[p - 1]++;
            if (!hud_panel(p)) {
                hud_ok = false;
                static const char *hdbg = getenv("XMEN2_MP_HUD");   /* diagnosing hud_missing: which panel, when */
                if (hdbg && hud_on)
                    fprintf(stderr, "[mp] frame %llu: player %d's HUD panel missing (scene %d)\n",
                            (unsigned long long)machine_frame_no(), p, (int)(int16_t)ram_be(0xCC64, 2));
                if (hud_on && C.n_miss < HUD_LIST) {
                    uint32_t *m = C.miss[C.n_miss++];
                    m[0] = frame, m[1] = (uint32_t)p, m[2] = level_frame == C.join_frame[p - 1] ? 1 : 0;
                } else if (hud_on) {
                    C.lists_full = 1;
                }
            } else if (hud_on) {   /* a stretch of frames with the panel: the last one grown, else a new one */
                if (p <= 2 && !hud_tiles(p)) {
                    C.tiles_wrong++;
                    if (getenv("XMEN2_MP_HUD"))
                        fprintf(stderr, "[mp] frame %llu: player %d's HUD panel tiles wrong (scene %d)\n",
                                (unsigned long long)machine_frame_no(), p, (int)(int16_t)ram_be(0xCC64, 2));
                    if (C.n_wrong < HUD_LIST) {
                        uint32_t *m = C.wrong[C.n_wrong++];
                        m[0] = frame, m[1] = (uint32_t)p, m[2] = level_frame == C.join_frame[p - 1] ? 1 : 0;
                    } else {
                        C.lists_full = 1;
                    }
                }
                uint32_t k = C.open_panel[p - 1];
                if (k && C.panel[k - 1][2] + 1 == frame) {
                    C.panel[k - 1][2] = frame;
                } else if (C.n_panel < HUD_LIST) {
                    uint32_t *s = C.panel[C.n_panel++];
                    s[0] = (uint32_t)p, s[1] = frame, s[2] = frame;
                    C.open_panel[p - 1] = C.n_panel;
                } else {
                    C.lists_full = 1;
                }
            }
        }
        if (n > C.players_max) C.players_max = n;
        if (!hud_ok && hud_on) C.hud_missing++;
        C.in_play_prev = in_play;
    } else {
        C.in_play_prev = 0;
    }
    s_marks.clear();
}

/* the second sound machine (R9): not built yet */

std::string players_counters_json(void)
{
    char b[640];
    auto l4 = [](const uint64_t *v) {
        char t[96];
        snprintf(t, sizeof t, "[%llu, %llu, %llu, %llu]", (unsigned long long)v[0], (unsigned long long)v[1],
                 (unsigned long long)v[2], (unsigned long long)v[3]);
        return std::string(t);
    };
    snprintf(b, sizeof b, "{\"gameplay_frames\": %llu, \"players_max\": %llu, \"in_play\": %s, \"drawn\": %s, "
             "\"off_view\": %s, \"hud_missing\": %llu, \"sprites_dropped\": %llu, \"snd2\": ",
             (unsigned long long)C.gameplay, (unsigned long long)C.players_max, l4(C.in_play).c_str(),
             l4(C.drawn).c_str(), l4(C.off_view).c_str(), (unsigned long long)C.hud_missing,
             (unsigned long long)C.dropped);
    auto list = [](const uint32_t (*v)[3], uint32_t n) {
        std::string s = "[";
        char t[48];
        for (uint32_t i = 0; i < n; i++) {
            snprintf(t, sizeof t, "%s[%u, %u, %u]", i ? ", " : "", (unsigned)v[i][0], (unsigned)v[i][1], (unsigned)v[i][2]);
            s += t;
        }
        return s + "]";
    };
    return std::string(b) + snd2_counters_json() + ", \"hud_missing_at\": " + list(C.miss, C.n_miss)
           + ", \"hud_panels\": " + list(C.panel, C.n_panel) + ", \"hud_tiles_wrong\": " + std::to_string(C.tiles_wrong)
           + ", \"hud_tiles_wrong_at\": " + list(C.wrong, C.n_wrong) + ", \"hud_lists_full\": " + (C.lists_full ? "1" : "0") + "}";
}

void players_state_init(void)
{
    state_counters(&C, sizeof C);
    STATE_VAR("platform", s_p34);
}
