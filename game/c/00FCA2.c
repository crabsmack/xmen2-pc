/* AvalonB1_CyclePalettes - one frame of the two AvalonB1 palette cycles (Pal_CycleStep):
 * Pal_AvalonB1Cycle on counters 5, 3, 4 and Pal_AvalonB1Cycle2 on counters 2, 0, 1 of
 * g_AvalonB1PalCycles. */
#include "xmen2.h"

void AvalonB1_CyclePalettes(void)
{
	Pal_CycleStep(&g_AvalonB1PalCycles[5], &g_AvalonB1PalCycles[3], &g_AvalonB1PalCycles[4],
		CODEPTR(Pal_AvalonB1Cycle), 1);
	Pal_CycleStep(&g_AvalonB1PalCycles[2], &g_AvalonB1PalCycles[0], &g_AvalonB1PalCycles[1],
		CODEPTR(Pal_AvalonB1Cycle2), 1);
}
