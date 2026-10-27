/* xmen2_options.h - the native runner's enhancement options (xmen2disasm docs/phases/P6-enhance.md
 * R1): set by platform/native/options.cpp from the command line and xmen2.ini, read by the platform
 * and by the game's native-only branches (`#if defined(XMEN2_NATIVE)`). All 0 = the original
 * (players: 2); music_voices, presentation only, is on by default. */
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
    int widescreen_gameplay;   /* R6/R8: with widescreen, objects activate across the wide view and the
                                  camera keeps it inside the level (0: the 16:9 view is presentation only) */
    int music_voices;   /* the music on sound machines of its own (platform/native/sound2.cpp), so no part
                           or effect takes its voices; on by default, 0 = the original's one machine */
} XmOptions;

extern XmOptions g_opt;

/* widescreen (platform/native/widescreen.cpp), on the running screen (all 0 on a 4:3 screen, or
   with the option off): how far past the 320-px view the game keeps the planes drawn, per side
   (the streamer; R8's render camera can move the window by up to a margin, so presentation keeps
   two); the drawn window's margins left and right of the 320 px this frame (VIEW_L/VIEW_R, the
   HUD); and the gameplay margin of R6, for what activates and the camera's limits (0 under R8) */
int plat_ws_view(void);
int plat_ws_left(void);
int plat_ws_right(void);
int plat_ws_act(void);
int plat_ws_hud(void);   /* the HUD's margin per side: the view's, whatever the render camera does */
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
   (its panel, its character's portrait, from the ROM's art; its health and meter strips as
   HUD_UpdateHealthBars draws them) */
int  plat_p34_dma(unsigned long src, unsigned short dest, unsigned short len);
void plat_spr_dropped(int n);   /* sprite pieces the stop (SPR_STOP) left out: the mp counters */
void plat_p34_hud(int k, short health, short charge, int hasMeter, unsigned short frame);
/* ... and with three or more players, player k's (1-4) character this frame (Player_CharIndexOf, -1 out
   of play): every player's HUD panel is its portrait */
void plat_hud_char(int k, int ch);

#ifdef __cplusplus
}
#endif

#endif /* XMEN2_OPTIONS_H */
