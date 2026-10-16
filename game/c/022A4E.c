/* AvalonA2_LoadLevel - load the Falling Room (AvalonA2): its level data through Level_Load (FG map
 * Map_AvalonA2Eni, FG tiles Art_AvalonA2Nem, BG map and tiles Art_AvalonA2Nem2, FG floor segments
 * Art_AvalonA2Nem3, no BG segments, flags 0 0 1 1, class-init argument $A, music 1, grid shifts 8
 * and 8, class list Map_AvalonA2Eni2), then palette colour 0 black, palette line 2 from
 * Pal_AvalonA2Line2 (16 colours), one-player stepping. */
#include "xmen2.h"

void AvalonA2_LoadLevel(void)
{
	register s16 *src;
	register s16 *dst;
	register long i;

	Level_Load(CODEPTR(Map_AvalonA2Eni), CODEPTR(Art_AvalonA2Nem), CODEPTR(Art_AvalonA2Nem2),
		CODEPTR(Art_AvalonA2Nem2), CODEPTR(Art_AvalonA2Nem3), 0, 0, 0, 1, 1, 0xA, 1, 8, 8, 0,
		CODEPTR(Map_AvalonA2Eni2));
	g_PalBase[0] = 0;
	src = (s16 *)CODEPTR(Pal_AvalonA2Line2);
	dst = &g_PalBase[0x20];
	for (i = 0; i < 16; i++)
		*dst++ = *src++;
	g_TwoPlayerStep = 0;
}
