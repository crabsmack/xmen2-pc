/* BaniMazaB2_LoadLevel - load BaniMazaB2 (Exterior): its level data through Level_Load (FG map and
 * tiles Art_BaniMazaB2Nem2, BG map and tiles Art_BaniMazaB2Nem4, FG floor segments
 * Art_BaniMazaB2Nem, no BG segments, flags 0 0 1 1, class-init argument $14, no music (-1), grid
 * shifts 8 and 8, class list Art_BaniMazaB2Nem3); then a fixed background (scroll factors 0), the
 * start camera offsets $B8 (g_LevelLoadWord3898) and $C0, the HUD on and one-player stepping. */
#include "xmen2.h"

void BaniMazaB2_LoadLevel(void)
{
	Level_Load(CODEPTR(Art_BaniMazaB2Nem2), CODEPTR(Art_BaniMazaB2Nem2), CODEPTR(Art_BaniMazaB2Nem4),
		CODEPTR(Art_BaniMazaB2Nem4), CODEPTR(Art_BaniMazaB2Nem), 0, 0, 0, 1, 1, 0x14, -1, 8, 8, 0,
		CODEPTR(Art_BaniMazaB2Nem3));
	g_BgScrollFactorY = 0;
	g_BgScrollFactorX = 0;
	g_LevelLoadWord3898 = 0xB8;
	g_StartCamOffsetY = 0xC0;
	g_HudOn = 1;
	g_TwoPlayerStep = 0;
}
