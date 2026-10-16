/* SavageC1_LoadLevel - load Jungle 1 (SavageC1): its level data through Level_Load (FG map and
 * tiles Art_Nem_0C1DDC, BG map and tiles Art_SpaceB1Nem, FG floor segments Lvl_Segs_0C14E4, no BG
 * segments, flags 0 0 1 0, class-init argument $14, no music (-1), grid shifts 8 and 8, class list
 * Art_Nem_0CA756), then background scroll factors $100 (y) and $8C (x), plane B wrapping in x,
 * one-player stepping (set, then cleared at once) and the HUD on. */
#include "xmen2.h"

void SavageC1_LoadLevel(void)
{
	Level_Load(CODEPTR(Art_Nem_0C1DDC), CODEPTR(Art_Nem_0C1DDC), CODEPTR(Art_SpaceB1Nem),
		CODEPTR(Art_SpaceB1Nem), CODEPTR(Lvl_Segs_0C14E4), 0, 0, 0, 1, 0, 0x14, -1, 8, 8, 0,
		CODEPTR(Art_Nem_0CA756));
	g_BgScrollFactorY = 0x100;
	g_BgScrollFactorX = 0x8C;
	g_BgWrapX = 1;
	g_TwoPlayerStep = 1;
	g_TwoPlayerStep = 0;
	g_HudOn = 1;
}
