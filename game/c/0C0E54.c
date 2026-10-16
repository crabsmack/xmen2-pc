/* SavageC1_LoadLevel - load Jungle 1 (SavageC1): its level data through Level_Load (FG map and
 * tiles Lvl_SavageC1Fg, BG map and tiles Lvl_SavageC1Bg, FG floor segments Lvl_SavageC1FgSegs, no BG
 * segments, flags 0 0 1 0, class-init argument $14, no music (-1), grid shifts 8 and 8, class list
 * Tbl_SavageC1ClassList), then background scroll factors $100 (y) and $8C (x), plane B wrapping in x,
 * one-player stepping (set, then cleared at once) and the HUD on. */
#include "xmen2.h"

void SavageC1_LoadLevel(void)
{
	Level_Load(CODEPTR(Lvl_SavageC1Fg), CODEPTR(Lvl_SavageC1Fg), CODEPTR(Lvl_SavageC1Bg),
		CODEPTR(Lvl_SavageC1Bg), CODEPTR(Lvl_SavageC1FgSegs), 0, 0, 0, 1, 0, 0x14, -1, 8, 8, 0,
		CODEPTR(Tbl_SavageC1ClassList));
	g_BgScrollFactorY = 0x100;
	g_BgScrollFactorX = 0x8C;
	g_BgWrapX = 1;
	g_TwoPlayerStep = 1;
	g_TwoPlayerStep = 0;
	g_HudOn = 1;
}
