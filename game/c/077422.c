/* CloneA1_CyclePalettes - one frame of the Factory Floor's two palette cycles (Pal_CloneA1Cycle,
 * Pal_CloneA1Cycle2) on one shared state; Pal_CycleStep keeps the new state only for the second.
 * (config/compiler/match/077422.c in xmen2disasm is the same function, written before the
 * headers existed.) */
#include "xmen2.h"

#define g_PalCycleTimer		(*(s16 *)g_SceneVar3B32)	/* scene variable (CloneA1): Pal_CycleStep timer */
#define g_PalCycleColour	(*(s16 *)g_SceneVar3B42)	/* scene variable (CloneA1): Pal_CycleStep colour index */
#define g_PalCycleSeq		(*(s16 *)g_SceneVar3B3A)	/* scene variable (CloneA1): Pal_CycleStep sequence index */

void CloneA1_CyclePalettes(void)
{
	Pal_CycleStep(&g_PalCycleTimer, &g_PalCycleColour, &g_PalCycleSeq, CODEPTR(Pal_CloneA1Cycle), 0);
	Pal_CycleStep(&g_PalCycleTimer, &g_PalCycleColour, &g_PalCycleSeq, CODEPTR(Pal_CloneA1Cycle2), 1);
}
