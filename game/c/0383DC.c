/* BaniMazaA1_LoadLevel - load the Moving Blocks stage (BaniMazaA1): its level data through
 * Level_Load (FG map and tiles Art_BaniMazaA1Nem, BG map and tiles Lvl_BaniMazaA1Data, FG floor
 * segments Lvl_BaniMazaA1Segs, no BG segments, flags 0 0 1 0, class-init argument $14, music 6,
 * grid shifts 8 and 8, class list Art_BaniMazaA1Nem2), then plane B scrolled 1:1 with the
 * camera and wrapped in x, one-player stepping (set to 1, then 0), the HUD on. */
#include "xmen2.h"

void BaniMazaA1_LoadLevel(void)
{
	Level_Load(CODEPTR(Art_BaniMazaA1Nem), CODEPTR(Art_BaniMazaA1Nem), CODEPTR(Lvl_BaniMazaA1Data),
		CODEPTR(Lvl_BaniMazaA1Data), CODEPTR(Lvl_BaniMazaA1Segs), 0, 0, 0, 1, 0, 0x14, 6, 8, 8, 0,
		CODEPTR(Art_BaniMazaA1Nem2));
	g_BgScrollFactorY = 0x100;
	g_BgScrollFactorX = 0x100;
	g_BgWrapX = 1;
	g_TwoPlayerStep = 1;
	g_TwoPlayerStep = 0;
	g_HudOn = 1;
}
