/* SiberiaB1_LoadLevel - load the Blizzard (SiberiaB1) through Level_Load: FG map and tiles
 * Lvl_SiberiaB1Fg, BG map and tiles Lvl_SiberiaB1Bg, no FG floor segments, BG segments
 * Lvl_SiberiaB1BgSegs, flags 1 0 1 0, class-init argument $14, no music (-1), grid shifts 7 and
 * 7, spawn list from the BG map, class list Tbl_SiberiaB1ClassList; then the HUD on, plane B scroll
 * factors $100 (1:1) and every scroll base 0. */
#include "xmen2.h"

void SiberiaB1_LoadLevel(void)
{
	register unsigned char flag0 = 1;
	register unsigned char flag1 = 0;
	register unsigned char flag2 = 1;
	register unsigned char flag3 = 0;

	Level_Load(CODEPTR(Lvl_SiberiaB1Fg), CODEPTR(Lvl_SiberiaB1Fg), CODEPTR(Lvl_SiberiaB1Bg),
		CODEPTR(Lvl_SiberiaB1Bg), 0, CODEPTR(Lvl_SiberiaB1BgSegs), flag0, flag1, flag2, flag3,
		0x14, -1, 7, 7, 1, CODEPTR(Tbl_SiberiaB1ClassList));
	g_HudOn = 1;
	g_BgScrollFactorX = 0x100;
	g_BgScrollFactorY = 0x100;
	g_FgScrollBaseY = 0;
	g_BgScrollBaseY = 0;
	g_FgScrollBaseX = 0;
	g_BgScrollBaseX = 0;
}
