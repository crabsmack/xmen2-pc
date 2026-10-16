/* BaniMazaA2_LoadLevel - load the Apocalypse room (BaniMazaA2): its level data through Level_Load
 * (FG map and tiles Art_BaniMazaA2Nem, BG map and tiles Art_BaniMazaA2Nem3, FG floor segments
 * Lvl_BaniMazaA2Data, no BG segments, flags 1 0 1 0, class-init argument $F, music $40, grid
 * shifts 8 and 8, class list Art_BaniMazaA2Nem2), then plane B scrolled 1:1 with the camera, the
 * HUD on, one-player stepping, and the scene's frame and draw hooks (BaniMazaA2_FrameHook,
 * BaniMazaA2_DrawHook). */
#include "xmen2.h"

void BaniMazaA2_LoadLevel(void)
{
	Level_Load(CODEPTR(Art_BaniMazaA2Nem), CODEPTR(Art_BaniMazaA2Nem), CODEPTR(Art_BaniMazaA2Nem3),
		CODEPTR(Art_BaniMazaA2Nem3), CODEPTR(Lvl_BaniMazaA2Data), 0, 1, 0, 1, 0, 0xF, 0x40, 8, 8, 0,
		CODEPTR(Art_BaniMazaA2Nem2));
	g_BgScrollFactorY = 0x100;
	g_BgScrollFactorX = 0x100;
	g_HudOn = 1;
	g_TwoPlayerStep = 0;
	g_SceneFrameHook = BaniMazaA2_FrameHook;
	g_SceneDrawHook = FN_DATA(BaniMazaA2_DrawHook);
}
