/* SiberiaB1_ResetPalCycles - restart the Blizzard's palette cycles (SiberiaB1_CyclePalettes):
 * their timer, colour index and sequence index 0. */
#include "xmen2.h"

#define g_PalCycleTimer	(*(s16 *)g_SceneVar3B34)	/* scene variable: Pal_CycleStep timer */
#define g_PalCycleColour	(*(s16 *)g_SceneVar3B44)	/* scene variable: Pal_CycleStep colour index */
#define g_PalCycleSeq	(*(s16 *)g_SceneVar3B3C)	/* scene variable: Pal_CycleStep sequence index */

void SiberiaB1_ResetPalCycles(void)
{
	g_PalCycleTimer = 0;
	g_PalCycleColour = 0;
	g_PalCycleSeq = 0;
}
