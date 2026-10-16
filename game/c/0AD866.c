/* SavageA2_LoadLevel - load the Perimeter, part 1 (SavageA2): its level data through Level_Load (FG
 * map and FG tiles both Lvl_SavageA2Fg, BG map and tiles Lvl_SavageA2Bg, FG floor segments
 * Lvl_SavageA2FgSegs, no BG segments, flags 0 1 1 1, class-init argument $14, music $6F, grid
 * shifts 8 and 8, class list Tbl_SavageA2ClassList), then colour 0 black, the HUD on, one-player
 * stepping, no BG wrap in y, the camera's lowest y $60 px higher (and its target with it), and
 * background scroll factors $140 (x) and $120 (y). */
#include "xmen2.h"

void SavageA2_LoadLevel(void)
{
	Level_Load(CODEPTR(Lvl_SavageA2Fg), CODEPTR(Lvl_SavageA2Fg), CODEPTR(Lvl_SavageA2Bg),
		CODEPTR(Lvl_SavageA2Bg), CODEPTR(Lvl_SavageA2FgSegs), 0, 0, 1, 1, 1, 0x14, 0x6F, 8, 8, 0,
		CODEPTR(Tbl_SavageA2ClassList));
	g_PalBase[0] = 0;
	g_HudOn = 1;
	g_TwoPlayerStep = 0;
	g_BgWrapY = 0;
	g_CamMaxY -= 0x60;
	g_CamMaxYTarget = g_CamMaxY;
	g_BgScrollFactorX = 0x140;
	g_BgScrollFactorY = 0x120;
}
