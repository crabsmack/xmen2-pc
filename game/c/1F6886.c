/* Pal_ClearBase - set the 64-colour base palette (g_PalBase) to black (Pal_FadeOutWait). */
#include "xmen2.h"

void Pal_ClearBase(void)
{
	char i;

	for (i = 0; i < 64; i++)
		g_PalBase[i] = 0;
}
