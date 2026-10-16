/* SavageA4_LoadLevel - load the Chamber (SavageA4): its level data through Level_Load (FG map and
 * tiles Lvl_SavageA4Fg, BG map and tiles Lvl_SavageA4Bg, FG floor segments Lvl_SavageA4FgSegs,
 * no BG segments, flags 0 0 1 0, class-init argument $14, music -1, grid shifts 8 and 8, class
 * list Tbl_SavageA4ClassList), then the HUD on, no BG wrap in y, the background fixed in x and
 * scrolled 1:1 in y, scroll bases 0 except the BG's y (100). */
#include "xmen2.h"

void SavageA4_LoadLevel(void)
{
	Level_Load(CODEPTR(Lvl_SavageA4Fg), CODEPTR(Lvl_SavageA4Fg), CODEPTR(Lvl_SavageA4Bg),
		CODEPTR(Lvl_SavageA4Bg), CODEPTR(Lvl_SavageA4FgSegs), 0, 0, 0, 1, 0, 0x14, -1, 8, 8, 0,
		CODEPTR(Tbl_SavageA4ClassList));
	g_HudOn = 1;
	g_BgWrapY = 0;
	g_BgScrollFactorX = 0;
	g_BgScrollFactorY = 0x100;
	g_FgScrollBaseY = 0;
	g_BgScrollBaseY = 100;
	g_FgScrollBaseX = 0;
	g_BgScrollBaseX = 0;
}
