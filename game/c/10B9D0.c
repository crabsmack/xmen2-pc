/* SpaceA1_LoadLevel - load the Space Elevator (SpaceA1): its level data through Level_Load (FG map
 * and tiles Lvl_SpaceA1Fg, BG map and tiles Lvl_SpaceA1Bg, FG floor segments Lvl_SpaceA1FgSegs,
 * no BG segments, flags 0 0 1 1, class-init argument $14, music $3E, grid shifts 8 and 8, class
 * list Tbl_SpaceA1ClassList), then palette colour 0 black, the HUD on, plane B not wrapping in x,
 * scroll factors 0 (y) and $100 (x), one-player stepping. */
#include "xmen2.h"

void SpaceA1_LoadLevel(void)
{
	Level_Load(CODEPTR(Lvl_SpaceA1Fg), CODEPTR(Lvl_SpaceA1Fg), CODEPTR(Lvl_SpaceA1Bg),
		CODEPTR(Lvl_SpaceA1Bg), CODEPTR(Lvl_SpaceA1FgSegs), 0, 0, 0, 1, 1, 0x14, 0x3E, 8, 8, 0,
		CODEPTR(Tbl_SpaceA1ClassList));
	g_PalBase[0] = 0;
	g_HudOn = 1;
	g_BgWrapX = 0;
	g_BgScrollFactorY = 0;
	g_BgScrollFactorX = 0x100;
	g_TwoPlayerStep = 0;
}
