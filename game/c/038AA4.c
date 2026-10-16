/* BaniMazaA1_InitPalette - the Moving Blocks' palette: line 3 from Art_BaniMazaA1Nem3 (after its
 * 4-byte header), colour 0 black, and colours 5, 7, 9 and 14 of line 2 ($068, $4AC, $ACE,
 * $022). */
#include "xmen2.h"

void BaniMazaA1_InitPalette(void)
{
	register s16 *src = (s16 *)CODEPTR(Art_BaniMazaA1Nem3) + 2;
	register s16 *dst = &g_PalBase[48];
	register long i;

	for (i = 0; i < 16; i++)
		*dst++ = *src++;
	g_PalBase[0] = 0;
	g_PalBase[37] = 0x68;
	g_PalBase[39] = 0x4AC;
	g_PalBase[41] = 0xACE;
	g_PalBase[46] = 0x22;
}
