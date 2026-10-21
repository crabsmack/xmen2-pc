/* AvalonA3_RunScene - the scene 'AvalonA3 Exodus': load the level, place the players, make the
 * camera, install the hooks, set up the effect sprites, save palette lines 0 and 2 for the
 * psychic flash (two buffers in this frame), then run the level loop (Level_Run) until the
 * scene ends.
 * PC: the buffers must be game memory (AvalonA3_PsiFlashStep reaches them through the scene
 * variables): they are placed on the 68000 stack where the original's frame had them (-$38(a6)
 * and -$1C(a6), a6 = A7 at entry - 4), with the frame's 8 more bytes (the saved a3/a4) reserved
 * below them for the calls made from here. */
#include "xmen2.h"

void AvalonA3_RunScene(void)
{
#ifdef XMEN2_MPW
	s16 buf0[14];
	s16 buf2[14];
	register s16 *pal0 = buf0;
	register s16 *pal2 = buf2;
#else
	unsigned long a7 = xm::frame_mark();
	s16 *pal0 = AT68(s16, xm::frame_alloc(4 + 0x38));
	s16 *pal2 = pal0 + 14;

	xm::frame_alloc(8);
#endif

	AvalonA3_EmptyStub1();
	AvalonA3_LoadLevel();
	AvalonA3_PlacePlayers();
	AvalonA3_InitCamera();
	AvalonA3_InstallHooks();
	AvalonA3_EmptyStub2();
	AvalonA3_InitSprites();
	AvalonA3_EmptyStub3();
	AvalonA3_SavePalettes(pal0, pal2);
	Level_Run();
#ifndef XMEN2_MPW
	xm::frame_release(a7);
#endif
}
