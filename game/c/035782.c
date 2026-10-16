/* AvalonA4_LoadLevel - load the Throne Room (AvalonA4, the Magneto fight): its level data through
 * Level_Load (FG map and tiles Lvl_AvalonA4Fg, BG map and tiles Lvl_AvalonA4Bg, FG floor
 * segments Lvl_AvalonA4FgSegs, no BG segments, flags 0 0 1 1, class-init argument $F, music $30,
 * grid shifts 8 and 8, class list Tbl_AvalonA4ClassList), then music 7 (restarted), palette colour 0
 * black, one-player stepping, background scroll factors $7F (y) and $A0 (x). */
#include "xmen2.h"

void AvalonA4_LoadLevel(void)
{
	Level_Load(CODEPTR(Lvl_AvalonA4Fg), CODEPTR(Lvl_AvalonA4Fg), CODEPTR(Lvl_AvalonA4Bg),
		CODEPTR(Lvl_AvalonA4Bg), CODEPTR(Lvl_AvalonA4FgSegs), 0, 0, 0, 1, 1, 0xF, 0x30, 8, 8, 0,
		CODEPTR(Tbl_AvalonA4ClassList));
	Snd_Stop(7);
	Snd_Play(7);
	g_PalBase[0] = 0;
	g_TwoPlayerStep = 0;
	g_BgScrollFactorY = 0x7F;
	g_BgScrollFactorX = 0xA0;
}
