/* xmen2_options.h - the native runner's enhancement options (xmen2disasm docs/phases/P6-enhance.md
 * R1): set by platform/native/options.cpp from the command line and xmen2.ini, read by the platform
 * and by the game's native-only branches (`#if defined(XMEN2_NATIVE)`). All 0 = the original. */
#ifndef XMEN2_OPTIONS_H
#define XMEN2_OPTIONS_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct XmOptions {
    int full_rate_2p;   /* R3: two players on the one-player stepping, no frame-tick wait */
    int high_fps;       /* R4: display rate in Hz (0 = off) */
    int widescreen;     /* R5-R7: 1 = 16:9 */
    int lives;          /* cheat: lives at a new game (0 = the original's 9), -1 = never spent */
    int magneto;        /* cheat: Magneto selectable from the start (the original: from Bani Maza) */
    int start_char;     /* cheat: player 1's character at power-on, 1 + g_P1CharIndex (0: at random) */
    int magneto_flight; /* tweak: Magneto moves left/right at his walking speed while hovering */
    int players;        /* phase 7 R7: players at once, 2 (the original), 3 or 4 */
} XmOptions;

extern XmOptions g_opt;

/* widescreen (platform/native/widescreen.cpp): the wide view's margin per side in px on the
   running screen (0: a 4:3 screen, or the option off), and the activation margin (R6) */
int plat_ws_view(void);
int plat_ws_act(void);
void plat_ws_cerebro(int on);   /* Cerebro_Run: a Cerebro screen is running (4:3, R5) */

/* high frame rate (platform/native/present.cpp): the sprites written from now on are owner's (a
   draw-list node's object, a timer, a scene hook), for matching them between frames (R4) */
void plat_spr_owner(const void *owner);
/* ... and the pieces an emitter writes from now on are sprite instance spr's, drawn at (x, y) in
   hardware sprite coordinates: each instance moves by its anchor's step, whatever its frame of
   animation does to the pieces */
void plat_spr_anchor(const void *spr, long x, long y);

/* development tests (options.cpp): XMEN2_DEBUG_SCENE=n makes scene n the first one of a new game
   (Game_ResetPlayers); without it, `scene` (the original) */
int plat_debug_first_scene(int scene);

/* four players (phase 7, platform/native/players.cpp): player k's (3, 4) code runs from now on (0: no
   more), for its sprites' tiles and the VRAM it writes; and players 3-4's pads from the host */
void plat_p34_enter(int k);
unsigned short plat_pad34(int k);
/* ... a DMA (68000 source, VRAM destination, bytes) into player 2's VRAM areas while a player 3-4's code
   runs: copied into that player's own tiles now (1), else 0 (the VDP's); and the player's HUD tiles
   (R6a: its panel from the ROM's art, its health and meter strips as HUD_UpdateHealthBars draws them) */
int  plat_p34_dma(unsigned long src, unsigned short dest, unsigned short len);
void plat_spr_dropped(int n);   /* sprite pieces the stop (SPR_STOP) left out: the mp counters */
void plat_p34_hud(int k, short health, short charge, int hasMeter, unsigned short frame);

#ifdef __cplusplus
}
#endif

#endif /* XMEN2_OPTIONS_H */
