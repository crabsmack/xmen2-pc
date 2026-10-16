/* SavageA4_ResetPalCycles - clear the Chamber's palette-cycle state (SavageA4_CyclePalettes) and a
 * second set of three words next to it that nothing in this scene uses. */
#include "xmen2.h"

#define g_A4CycleTimer	(*(s16 *)g_SceneVar3B34)	/* scene variable: Pal_CycleStep timer */
#define g_A4CycleColour	(*(s16 *)g_SceneVar3B44)	/* scene variable: Pal_CycleStep colour index */
#define g_A4CycleSeq	(*(s16 *)g_SceneVar3B3C)	/* scene variable: Pal_CycleStep sequence index */
#define g_A4Unused36	(*(s16 *)g_SceneVar3B36)	/* scene variables: cleared here, nothing in SavageA4 reads them */
#define g_A4Unused46	(*(s16 *)g_SceneVar3B46)
#define g_A4Unused3E	(*(s16 *)g_SceneVar3B3E)

void SavageA4_ResetPalCycles(void)
{
	g_A4CycleTimer = 0;
	g_A4CycleColour = 0;
	g_A4CycleSeq = 0;
	g_A4Unused36 = 0;
	g_A4Unused46 = 0;
	g_A4Unused3E = 0;
}
