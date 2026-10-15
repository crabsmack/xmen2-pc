/* SiberiaB1_CyclePalettes - one frame of the Blizzard's three palette cycles
 * (Tbl_SiberiaB1PalCycle1-3, one shared state; Pal_CycleStep keeps the new state only for the
 * third). */
#include "xmen2.h"

#define g_PalCycleTimer	(*(s16 *)g_SceneVar3B34)	/* scene variable: Pal_CycleStep timer */
#define g_PalCycleColour	(*(s16 *)g_SceneVar3B44)	/* scene variable: Pal_CycleStep colour index */
#define g_PalCycleSeq	(*(s16 *)g_SceneVar3B3C)	/* scene variable: Pal_CycleStep sequence index */

void SiberiaB1_CyclePalettes(void)
{
	Pal_CycleStep(&g_PalCycleTimer, &g_PalCycleColour, &g_PalCycleSeq, CODEPTR(Tbl_SiberiaB1PalCycle1), 0);
	Pal_CycleStep(&g_PalCycleTimer, &g_PalCycleColour, &g_PalCycleSeq, CODEPTR(Tbl_SiberiaB1PalCycle2), 0);
	Pal_CycleStep(&g_PalCycleTimer, &g_PalCycleColour, &g_PalCycleSeq, CODEPTR(Tbl_SiberiaB1PalCycle3), 1);
}
