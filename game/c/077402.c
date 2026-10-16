/* CloneA1_ResetPalCycle - restart the Factory Floor's palette cycles (CloneA1_CyclePalettes): both
 * Pal_CycleStep states (timer, colour index, sequence index) to 0; the second is never cycled. */
#include "xmen2.h"

#define g_PalCycleTimer		(*(s16 *)g_SceneVar3B32)	/* scene variable (CloneA1): Pal_CycleStep timer */
#define g_PalCycleColour	(*(s16 *)g_SceneVar3B42)	/* scene variable (CloneA1): Pal_CycleStep colour index */
#define g_PalCycleSeq		(*(s16 *)g_SceneVar3B3A)	/* scene variable (CloneA1): Pal_CycleStep sequence index */
#define g_PalCycle2Timer	(*(s16 *)g_SceneVar3B34)	/* scene variables (CloneA1): a second state, only reset */
#define g_PalCycle2Colour	(*(s16 *)g_SceneVar3B44)
#define g_PalCycle2Seq		(*(s16 *)g_SceneVar3B3C)

void CloneA1_ResetPalCycle(void)
{
	g_PalCycleTimer = 0;
	g_PalCycleColour = 0;
	g_PalCycleSeq = 0;
	g_PalCycle2Timer = 0;
	g_PalCycle2Colour = 0;
	g_PalCycle2Seq = 0;
}
