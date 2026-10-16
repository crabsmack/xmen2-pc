/* Pal_RestoreBase - copy the palette saved by Pal_SaveBase back into g_PalBase (Pal_FadeInWait). */
#include "xmen2.h"

/* Defined in the original's file (see Pal_SaveBase): MPW keeps its address in a register. */
#ifdef XMEN2_MPW
s16 g_PalSave[64];
#endif

void Pal_RestoreBase(void)
{
	char i;

	for (i = 0; i < 64; i++)
		g_PalBase[i] = g_PalSave[i];
}
