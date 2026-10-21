/* AvalonB1_RunScene - the scene "AvalonB1 Exterior" (Scene_RegisterAll): load the level, place
 * the players, run the camera intro and the level until it ends.
 *
 * Level_Load: FG map and tiles Lvl_AvalonB1Fg, BG map and tiles Lvl_AvalonB1Bg, FG floor
 * segments Lvl_AvalonB1FgSegs, no BG segments, flags 1 0 1 0, class-init argument $F, music $5C,
 * grid shifts 8 and 8, class list Tbl_AvalonB1ClassList. Then the HUD on, plane B fixed (scroll
 * factors 0), the players at their start (Players_PlaceAtStart), the tile hooks, the frame hook
 * AvalonB1_FrameHook, one-player stepping, the palette cycles reset, the intro camera
 * (AvalonB1_InitCamera), the gauss bullet pool; the level runs with the sound muted (the intro
 * unmutes it), then every sound stops.
 * Palette line 0 is saved for the intro in a buffer of this frame (g_SceneVar3B14).
 * MPW: matches (CLEAR_COUNTER's if (1) keeps MPW from merging adjacent clears into clr.l).
 * PC: the buffer must be game memory (other functions reach it through g_SceneVar3B14): it is
 * placed on the 68000 stack where the original's frame had it (-$20(a6), a6 = A7 at entry - 4),
 * with that frame's 4 more bytes (the saved a4) reserved below it for the calls made from here. */
#include "xmen2.h"

#define g_AvalonB1SavedPal	(*(PTR(s16) *)g_SceneVar3B14)	/* scene variable: the buffer for palette line 0 (AvalonB1_InitCamera) */

/* The original's six counters were probably separate globals: MPW C merges two adjacent clears
   of one array into a clr.l, and the if ends a basic block after each clear (no code), which
   keeps the ROM's six clr.w. */
#define CLEAR_COUNTER(x)	if (1) (x) = 0

void AvalonB1_RunScene(void)
{
#ifdef XMEN2_MPW
	s16 pal[16];
#else
	unsigned long a7 = xm::frame_mark();
	s16 *pal = AT68(s16, xm::frame_alloc(4 + 0x20));

	xm::frame_alloc(4);
#endif

	g_AvalonB1SavedPal = pal;
	Level_Load(CODEPTR(Lvl_AvalonB1Fg), CODEPTR(Lvl_AvalonB1Fg), CODEPTR(Lvl_AvalonB1Bg),
		CODEPTR(Lvl_AvalonB1Bg), CODEPTR(Lvl_AvalonB1FgSegs), 0, 1, 0, 1, 0, 0xF, 0x5C, 8, 8, 0,
		CODEPTR(Tbl_AvalonB1ClassList));
	g_HudOn = 1;
	g_BgScrollFactorX = 0;
	g_BgScrollFactorY = 0;
	Players_PlaceAtStart(0, 0, 0, 0);
	AvalonB1_InstallTileHooks();
	g_SceneFrameHook = AvalonB1_FrameHook;
	g_TwoPlayerStep = 0;
	CLEAR_COUNTER(g_AvalonB1PalCycles[5]);	/* Pal_AvalonB1Cycle: timer, step, sequence */
	CLEAR_COUNTER(g_AvalonB1PalCycles[3]);
	CLEAR_COUNTER(g_AvalonB1PalCycles[4]);
	CLEAR_COUNTER(g_AvalonB1PalCycles[2]);	/* Pal_AvalonB1Cycle2 */
	CLEAR_COUNTER(g_AvalonB1PalCycles[0]);
	CLEAR_COUNTER(g_AvalonB1PalCycles[1]);
	AvalonB1_InitCamera();
	GaussBullet_InitPool();
	g_SndMute = 1;
	Level_Run();
	g_SndMute = 0;
	Snd_StopAll();
#ifndef XMEN2_MPW
	xm::frame_release(a7);
#endif
}
