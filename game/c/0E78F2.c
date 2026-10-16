/* SentinelsA1_LoadLevel - load the Coolant Core (SentinelsA1) through Level_Load: FG map and
 * tiles Lvl_SentinelsA1Fg, BG map and tiles Lvl_SentinelsA1Bg, FG floor segments
 * Lvl_SentinelsA1FgSegs, no BG segments, flags 0 0 1 0, class-init argument $14, music $23, grid
 * shifts 7 and 8, no spawn from the BG map, class list Tbl_SentinelsA1ClassList; then palette colour
 * 0 black, the HUD on, plane B scroll factors $80 (half speed), every scroll base 0, two-player
 * step mode on (the frame hook runs twice per frame).
 * The HUD flag and the step mode are each cleared, then set. A fourth flag variable is set to 0
 * and never used (the fourth flag is passed as a constant). */
#include "xmen2.h"

void SentinelsA1_LoadLevel(void)
{
	register unsigned char flag0 = 0;
	register unsigned char flag1 = 0;
	register unsigned char flag2 = 1;
	register unsigned char flag3 = 0;

	Level_Load(CODEPTR(Lvl_SentinelsA1Fg), CODEPTR(Lvl_SentinelsA1Fg), CODEPTR(Lvl_SentinelsA1Bg),
		CODEPTR(Lvl_SentinelsA1Bg), CODEPTR(Lvl_SentinelsA1FgSegs), 0, flag0, flag1, flag2, 0,
		0x14, 0x23, 7, 8, 0, CODEPTR(Tbl_SentinelsA1ClassList));
	g_PalBase[0] = 0;
	g_HudOn = 0;
	g_HudOn = 1;
	g_BgScrollFactorX = 0x80;
	g_BgScrollFactorY = 0x80;
	g_FgScrollBaseY = 0;
	g_BgScrollBaseY = 0;
	g_FgScrollBaseX = 0;
	g_BgScrollBaseX = 0;
	g_TwoPlayerStep = 0;
	g_TwoPlayerStep = 1;
}
