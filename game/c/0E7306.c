/* SentinelsA1_CyclePalettes - one frame of the Coolant Core's palette cycles (Pal_CycleStep):
 * Pal_SentinelsA1Cycle-6 on five states (Cycle3 and Cycle5 share one, only Cycle5 keeps its new
 * state), and once the exit is open (g_SentinelsA1ExitOpen, Hologram_OpenExit) the exit's 16
 * cycles Pal_SentinelsA1Cycle7-22 on one shared state (only the last keeps it). */
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

#define EXIT_CYCLE(pal, keep) \
	Pal_CycleStep(&g_SentinelsA1ExitPalTimer, &g_SentinelsA1ExitPalColour, &g_SentinelsA1ExitPalSeq, \
		CODEPTR(pal), keep)

void SentinelsA1_CyclePalettes(void)
{
	Pal_CycleStep(&g_PalTimer1, &g_PalColour1, &g_PalSeq1, CODEPTR(Pal_SentinelsA1Cycle), 1);
	Pal_CycleStep(&g_PalTimer2, &g_PalColour2, &g_PalSeq2, CODEPTR(Pal_SentinelsA1Cycle2), 1);
	Pal_CycleStep(&g_PalTimer3, &g_PalColour3, &g_PalSeq3, CODEPTR(Pal_SentinelsA1Cycle3), 0);
	Pal_CycleStep(&g_PalTimer3, &g_PalColour3, &g_PalSeq3, CODEPTR(Pal_SentinelsA1Cycle5), 1);
	Pal_CycleStep(&g_PalTimer4, &g_PalColour4, &g_PalSeq4, CODEPTR(Pal_SentinelsA1Cycle4), 1);
	Pal_CycleStep(&g_SentinelsA1PalTimer, &g_SentinelsA1PalColour, &g_SentinelsA1PalSeq,
		CODEPTR(Pal_SentinelsA1Cycle6), 1);
	if (g_SentinelsA1ExitOpen) {
		EXIT_CYCLE(Pal_SentinelsA1Cycle7, 0);
		EXIT_CYCLE(Pal_SentinelsA1Cycle8, 0);
		EXIT_CYCLE(Pal_SentinelsA1Cycle9, 0);
		EXIT_CYCLE(Pal_SentinelsA1Cycle10, 0);
		EXIT_CYCLE(Pal_SentinelsA1Cycle11, 0);
		EXIT_CYCLE(Pal_SentinelsA1Cycle12, 0);
		EXIT_CYCLE(Pal_SentinelsA1Cycle13, 0);
		EXIT_CYCLE(Pal_SentinelsA1Cycle14, 0);
		EXIT_CYCLE(Pal_SentinelsA1Cycle15, 0);
		EXIT_CYCLE(Pal_SentinelsA1Cycle16, 0);
		EXIT_CYCLE(Pal_SentinelsA1Cycle17, 0);
		EXIT_CYCLE(Pal_SentinelsA1Cycle18, 0);
		EXIT_CYCLE(Pal_SentinelsA1Cycle19, 0);
		EXIT_CYCLE(Pal_SentinelsA1Cycle20, 0);
		EXIT_CYCLE(Pal_SentinelsA1Cycle21, 0);
		EXIT_CYCLE(Pal_SentinelsA1Cycle22, 1);
	}
}
