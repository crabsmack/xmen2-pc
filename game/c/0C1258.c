/* SavageB1_LoadLevel - load Jungle 2 (SavageB1): its level data through Level_Load (FG map
 * Lvl_SavageB1FgMap, FG tiles Lvl_SavageC1Fg, BG map Lvl_SavageB1BgMap, BG tiles Lvl_SavageC1Bg, FG
 * floor segments Lvl_SavageB1FgSegs, no BG segments, flags 0 0 1 1, class-init argument $14, no
 * music (-1), grid shifts 8 and 8, class list Tbl_SavageB1ClassList), then background scroll factors
 * $100 (y) and $8C (x), background base y 0, plane B wrapping in x but not y, one-player stepping
 * (set, then cleared at once) and the HUD on. */
#include "xmen2.h"

void SavageB1_LoadLevel(void)
{
	Level_Load(CODEPTR(Lvl_SavageB1FgMap), CODEPTR(Lvl_SavageC1Fg), CODEPTR(Lvl_SavageB1BgMap),
		CODEPTR(Lvl_SavageC1Bg), CODEPTR(Lvl_SavageB1FgSegs), 0, 0, 0, 1, 1, 0x14, -1, 8, 8, 0,
		CODEPTR(Tbl_SavageB1ClassList));
	g_BgScrollFactorY = 0x100;
	g_BgScrollFactorX = 0x8C;
	g_BgScrollBaseY = 0;
	g_BgWrapX = 1;
	g_BgWrapY = 0;
	g_TwoPlayerStep = 1;
	g_TwoPlayerStep = 0;
	g_HudOn = 1;
}
