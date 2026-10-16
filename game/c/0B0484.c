/* SavageA3_LoadLevel - load the Perimeter, part 2 (SavageA3): its level data through Level_Load
 * (FG map Map_SavageA3Eni, FG tiles Art_SavageA2Nem, BG map and tiles Lvl_SavageA2Data, FG floor
 * segments Lvl_SavageA3Segs, no BG segments, flags 0 1 1 1, class-init argument $14, music $70,
 * grid shifts 8 and 8, class list Map_SavageA3Eni2), then colour 0 black, the HUD on,
 * one-player stepping, no BG wrap in y, the camera's lowest y $60 px higher (and its target with
 * it), and background scroll factors $140 (x) and $120 (y). */
#include "xmen2.h"

void SavageA3_LoadLevel(void)
{
	Level_Load(CODEPTR(Map_SavageA3Eni), CODEPTR(Art_SavageA2Nem), CODEPTR(Lvl_SavageA2Data),
		CODEPTR(Lvl_SavageA2Data), CODEPTR(Lvl_SavageA3Segs), 0, 0, 1, 1, 1, 0x14, 0x70, 8, 8, 0,
		CODEPTR(Map_SavageA3Eni2));
	g_PalBase[0] = 0;
	g_HudOn = 1;
	g_TwoPlayerStep = 0;
	g_BgWrapY = 0;
	g_CamMaxY -= 0x60;
	g_CamMaxYTarget = g_CamMaxY;
	g_BgScrollFactorX = 0x140;
	g_BgScrollFactorY = 0x120;
}
