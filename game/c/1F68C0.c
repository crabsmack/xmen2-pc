/* Pal_RestoreCycle - put back the palette-cycle script Pal_SaveCycle saved (Pal_FadeInWait). */
#include "xmen2.h"

void Pal_RestoreCycle(void)
{
	g_PalCycleScript = g_PalCycleSave;
}
