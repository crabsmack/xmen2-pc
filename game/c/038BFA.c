/* BaniMazaA1_RunScene - the scene 'BaniMazaA1 Moving_Blocks' (also attract demo 0): load the
 * level, the block sprites, the tile hooks, the colour sequences, the disc scrolling, the frame
 * hook, the players, the palette and the camera, run the level loop (Level_Run) until the scene
 * ends, then H-scroll back to whole-screen (VDP register 11 = g_VdpReg11 + 0: the H-scroll mode
 * added, as BaniMazaA1_InitScroll's + 3; the ROM computes it in d0). */
#include "xmen2.h"

void BaniMazaA1_RunScene(void)
{
	BaniMazaA1_EmptyStub();
	BaniMazaA1_LoadLevel();
	BaniMazaA1_InitSprites();
	BaniMazaA1_InstallTileHooks();
	BaniMazaA1_InitTablePtrs();
	BaniMazaA1_InitScroll();
	BaniMazaA1_InstallFrameHook();
	BaniMazaA1_PlacePlayers();
	BaniMazaA1_InitPalette();
	BaniMazaA1_InitCamera();
	Level_Run();
	VDP_CTRL = g_VdpReg11 + 0;
}
