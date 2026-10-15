/* XClone_SetBgPalette - load one of the three clone background palettes into palette line 2.
 *
 * n: 1 Pal_XCloneBg1 (also any other value), 2 Pal_XCloneBg2, 3 Pal_XCloneBg3 (the EVIL* Inits). */
#include "xmen2.h"

void XClone_SetBgPalette(short n)
{
	s16 *pal;

	switch (n) {
	case 3:
		pal = (s16 *)CODEPTR(Pal_XCloneBg3);
		break;
	case 2:
		pal = (s16 *)CODEPTR(Pal_XCloneBg2);
		break;
	case 1:
	default:
		pal = (s16 *)CODEPTR(Pal_XCloneBg1);
		break;
	}
	Mem_Copy(&g_PalBase[0x20], pal, 0x20);
	g_PalBaseDirty = 1;
}
