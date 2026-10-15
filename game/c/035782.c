/* AvalonA4_LoadLevel - load the Throne Room (AvalonA4, the Magneto fight): its level data through
 * Level_Load (FG map and tiles Art_AvalonA4Nem_2, BG map and tiles Art_AvalonA4Nem, FG floor
 * segments Art_AvalonA4Nem2, no BG segments, flags 0 0 1 1, class-init argument $F, music $30,
 * grid shifts 8 and 8, class list Art_AvalonA4Nem3), then music 7 (restarted), palette colour 0
 * black, one-player stepping, background scroll factors $7F (y) and $A0 (x). */
#include "xmen2.h"

void AvalonA4_LoadLevel(void)
{
	Level_Load(CODEPTR(Art_AvalonA4Nem_2), CODEPTR(Art_AvalonA4Nem_2), CODEPTR(Art_AvalonA4Nem),
		CODEPTR(Art_AvalonA4Nem), CODEPTR(Art_AvalonA4Nem2), 0, 0, 0, 1, 1, 0xF, 0x30, 8, 8, 0,
		CODEPTR(Art_AvalonA4Nem3));
	Snd_Stop(7);
	Snd_Play(7);
	g_PalBase[0] = 0;
	g_TwoPlayerStep = 0;
	g_BgScrollFactorY = 0x7F;
	g_BgScrollFactorX = 0xA0;
}
