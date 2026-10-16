/* Pal_SaveCycle - remember the running palette-cycle script (g_PalCycleScript) in g_PalCycleSave
 * (Pal_FadeOutWait; Pal_RestoreCycle puts it back). */
#include "xmen2.h"

void Pal_SaveCycle(void)
{
	g_PalCycleSave = g_PalCycleScript;
}
