/* BaniMazaB2_InitPalette - palette line 3 = the 16 colours of Art_TusketteSpr (TUSKETTE's
 * sprite definition, type $19, whose header carries its palette from +4; BaniMazaA1 uses the
 * same). Not marked for upload here. */
#include "xmen2.h"

void BaniMazaB2_InitPalette(void)
{
	register s16 *src;
	register s16 *dst;
	register long i;

	src = (s16 *)CODEPTR(Art_TusketteSpr) + 2;
	dst = &g_PalBase[0x30];
	for (i = 0; i < 16; i++)
		*dst++ = *src++;
}
