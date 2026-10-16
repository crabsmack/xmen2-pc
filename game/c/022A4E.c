/* AvalonA2_LoadLevel - load the Falling Room (AvalonA2): its level data through Level_Load (FG map
 * Lvl_AvalonA2FgMap, FG tiles Lvl_AvalonB1Fg, BG map and tiles Lvl_AvalonA2Bg, FG floor segments
 * Lvl_AvalonA2FgSegs, no BG segments, flags 0 0 1 1, class-init argument $A, music 1, grid shifts 8
 * and 8, class list Tbl_AvalonA2ClassList), then palette colour 0 black, palette line 2 from
 * Pal_AvalonA2Line2 (16 colours), one-player stepping. */
#include "xmen2.h"

void AvalonA2_LoadLevel(void)
{
	register s16 *src;
	register s16 *dst;
	register long i;

	Level_Load(CODEPTR(Lvl_AvalonA2FgMap), CODEPTR(Lvl_AvalonB1Fg), CODEPTR(Lvl_AvalonA2Bg),
		CODEPTR(Lvl_AvalonA2Bg), CODEPTR(Lvl_AvalonA2FgSegs), 0, 0, 0, 1, 1, 0xA, 1, 8, 8, 0,
		CODEPTR(Tbl_AvalonA2ClassList));
	g_PalBase[0] = 0;
	src = (s16 *)CODEPTR(Pal_AvalonA2Line2);
	dst = &g_PalBase[0x20];
	for (i = 0; i < 16; i++)
		*dst++ = *src++;
	g_TwoPlayerStep = 0;
}
