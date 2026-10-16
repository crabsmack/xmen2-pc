/* SavageA4_CyclePalettes - one frame of the Chamber's three palette cycles (Pal_SavageA4Cycle,
 * Pal_SavageA4Cycle2, Pal_SavageA4Cycle3) on one shared state; Pal_CycleStep keeps the new state
 * only for the third. */
#include "xmen2.h"

#define g_A4CycleTimer	(*(s16 *)g_SceneVar3B34)	/* scene variable: Pal_CycleStep timer */
#define g_A4CycleColour	(*(s16 *)g_SceneVar3B44)	/* scene variable: Pal_CycleStep colour index */
#define g_A4CycleSeq	(*(s16 *)g_SceneVar3B3C)	/* scene variable: Pal_CycleStep sequence index */

void SavageA4_CyclePalettes(void)
{
	Pal_CycleStep(&g_A4CycleTimer, &g_A4CycleColour, &g_A4CycleSeq, CODEPTR(Pal_SavageA4Cycle), 0);
	Pal_CycleStep(&g_A4CycleTimer, &g_A4CycleColour, &g_A4CycleSeq, CODEPTR(Pal_SavageA4Cycle2), 0);
	Pal_CycleStep(&g_A4CycleTimer, &g_A4CycleColour, &g_A4CycleSeq, CODEPTR(Pal_SavageA4Cycle3), 1);
}
