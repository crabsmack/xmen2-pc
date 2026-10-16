/* SentinelsA1_ResetPalCycles - clear the state (timer, colour index, sequence index) of each of
 * the Coolant Core's palette cycles (SentinelsA1_CyclePalettes): its four scene-variable states
 * and its two own ones. */
#include "xmen2.h"

#define g_PalTimer1	(*(s16 *)g_SceneVar3B32)	/* scene variable: Pal_CycleStep state of Pal_SentinelsA1Cycle */
#define g_PalColour1	(*(s16 *)g_SceneVar3B42)
#define g_PalSeq1	(*(s16 *)g_SceneVar3B3A)
#define g_PalTimer2	(*(s16 *)g_SceneVar3B34)	/* scene variable: state of Pal_SentinelsA1Cycle2 */
#define g_PalColour2	(*(s16 *)g_SceneVar3B44)
#define g_PalSeq2	(*(s16 *)g_SceneVar3B3C)
#define g_PalTimer3	(*(s16 *)g_SceneVar3B36)	/* scene variable: state of Pal_SentinelsA1Cycle3 and 5 */
#define g_PalColour3	(*(s16 *)g_SceneVar3B46)
#define g_PalSeq3	(*(s16 *)g_SceneVar3B3E)
#define g_PalTimer4	(*(s16 *)g_SceneVar3B38)	/* scene variable: state of Pal_SentinelsA1Cycle4 */
#define g_PalColour4	(*(s16 *)g_SceneVar3B48)
#define g_PalSeq4	(*(s16 *)g_SceneVar3B40)

void SentinelsA1_ResetPalCycles(void)
{
	g_PalTimer1 = 0;
	g_PalColour1 = 0;
	g_PalSeq1 = 0;
	g_PalTimer2 = 0;
	g_PalColour2 = 0;
	g_PalSeq2 = 0;
	g_PalTimer3 = 0;
	g_PalColour3 = 0;
	g_PalSeq3 = 0;
	g_PalTimer4 = 0;
	g_PalColour4 = 0;
	g_PalSeq4 = 0;
	g_SentinelsA1PalTimer = 0;
	g_SentinelsA1PalColour = 0;
	g_SentinelsA1PalSeq = 0;
	g_SentinelsA1ExitPalTimer = 0;
	g_SentinelsA1ExitPalColour = 0;
	g_SentinelsA1ExitPalSeq = 0;
}
