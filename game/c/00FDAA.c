/* AvalonB1_RunScene - the scene "AvalonB1 Exterior" (Scene_RegisterAll): load the level, place
 * the players, run the camera intro and the level until it ends.
 *
 * Level_Load: FG map and tiles Art_AvalonA2Nem, BG map and tiles Map_AvalonB1Bg, FG floor
 * segments Art_AvalonB1Nem2, no BG segments, flags 1 0 1 0, class-init argument $F, music $5C,
 * grid shifts 8 and 8, class list Art_AvalonB1Nem3. Then the HUD on, plane B fixed (scroll
 * factors 0), the players at their start (Players_PlaceAtStart), the tile hooks, the frame hook
 * AvalonB1_FrameHook, one-player stepping, the palette cycles reset, the intro camera
 * (AvalonB1_InitCamera), the gauss bullet pool; the level runs with the sound muted (the intro
 * unmutes it), then every sound stops.
 * Palette line 0 is saved for the intro in a buffer of this frame (g_SceneVar3B14).
 * MPW: not matched. The ROM keeps the buffer's address in a4 (saved) and clears the six cycle
 * counters with six clr.w; MPW merges the adjacent ones of the array into clr.l (the original
 * probably had six globals). Verified by shadow tests.
 * PC: the buffer must be game memory (other functions reach it through g_SceneVar3B14): it is
 * placed on the 68000 stack where the original's frame had it (-$20(a6), a6 = A7 at entry - 4),
 * with that frame's 4 more bytes (the saved a4) reserved below it for the calls made from here. */
#include "xmen2.h"

#define g_AvalonB1SavedPal	(*(PTR(s16) *)g_SceneVar3B14)	/* scene variable: the buffer for palette line 0 (AvalonB1_InitCamera) */

void AvalonB1_RunScene(void)
{
#ifdef XMEN2_MPW
	s16 pal[16];
#else
	unsigned long a7 = CPU_GetSP();
	s16 *pal = AT68(s16, xm::sp_reserve(4 + 0x20));

	xm::sp_reserve(4);
#endif

	g_AvalonB1SavedPal = pal;
	Level_Load(CODEPTR(Art_AvalonA2Nem), CODEPTR(Art_AvalonA2Nem), CODEPTR(Map_AvalonB1Bg),
		CODEPTR(Map_AvalonB1Bg), CODEPTR(Art_AvalonB1Nem2), 0, 1, 0, 1, 0, 0xF, 0x5C, 8, 8, 0,
		CODEPTR(Art_AvalonB1Nem3));
	g_HudOn = 1;
	g_BgScrollFactorX = 0;
	g_BgScrollFactorY = 0;
	Players_PlaceAtStart(0, 0, 0, 0);
	AvalonB1_InstallTileHooks();
	g_SceneFrameHook = AvalonB1_FrameHook;
	g_TwoPlayerStep = 0;
	g_AvalonB1PalCycles[5] = 0;	/* Pal_AvalonB1Cycle: timer, step, sequence */
	g_AvalonB1PalCycles[3] = 0;
	g_AvalonB1PalCycles[4] = 0;
	g_AvalonB1PalCycles[2] = 0;	/* Pal_AvalonB1Cycle2 */
	g_AvalonB1PalCycles[0] = 0;
	g_AvalonB1PalCycles[1] = 0;
	AvalonB1_InitCamera();
	GaussBullet_InitPool();
	g_SndMute = 1;
	Level_Run();
	g_SndMute = 0;
	Snd_StopAll();
#ifndef XMEN2_MPW
	xm::sp_restore(a7);
#endif
}
