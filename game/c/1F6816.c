/* Pal_SaveBase - copy the 64-colour base palette (g_PalBase) to g_PalSave (Pal_FadeOutWait). */
#include "xmen2.h"

/* The original's file (Pal_SaveBase .. Pal_FadeInWait) defined g_PalSave: MPW C keeps the
   address of an array defined in the compile unit in a register for the loop (lea g_PalSave(a5),a4,
   indexed .w), an extern one it addresses again at each use (g_PalBase). */
#ifdef XMEN2_MPW
s16 g_PalSave[64];
#endif

void Pal_SaveBase(void)
{
	char i;

	for (i = 0; i < 64; i++)
		g_PalSave[i] = g_PalBase[i];
}
