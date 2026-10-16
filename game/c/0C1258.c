/* SavageB1_LoadLevel - load Jungle 2 (SavageB1): its level data through Level_Load (FG map
 * Map_SavageB1Eni, FG tiles Art_Nem_0C1DDC, BG map Lvl_SavageB1Segs, BG tiles Art_SpaceB1Nem, FG
 * floor segments Lvl_SavageB1Data, no BG segments, flags 0 0 1 1, class-init argument $14, no
 * music (-1), grid shifts 8 and 8, class list Map_SavageB1Eni2), then background scroll factors
 * $100 (y) and $8C (x), background base y 0, plane B wrapping in x but not y, one-player stepping
 * (set, then cleared at once) and the HUD on. */
#include "xmen2.h"

void SavageB1_LoadLevel(void)
{
	Level_Load(CODEPTR(Map_SavageB1Eni), CODEPTR(Art_Nem_0C1DDC), CODEPTR(Lvl_SavageB1Segs),
		CODEPTR(Art_SpaceB1Nem), CODEPTR(Lvl_SavageB1Data), 0, 0, 0, 1, 1, 0x14, -1, 8, 8, 0,
		CODEPTR(Map_SavageB1Eni2));
	g_BgScrollFactorY = 0x100;
	g_BgScrollFactorX = 0x8C;
	g_BgScrollBaseY = 0;
	g_BgWrapX = 1;
	g_BgWrapY = 0;
	g_TwoPlayerStep = 1;
	g_TwoPlayerStep = 0;
	g_HudOn = 1;
}
