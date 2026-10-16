/* CerebroSentinels_RunScene - the scene 'CerebroScreen Sentinels' (the first Cerebro screen of a
 * game): the Cerebro intro, the Sentinels briefing (Str_CerebroSentinelsText) and the player
 * select (Cerebro_Run); then progress flag 2 (intro seen) is set. */
#include "xmen2.h"

#define g_CerebroIntro		(*(s16 *)g_SceneVar3B0C)	/* scene variable: Cerebro_Run plays the intro first */
#define g_CerebroSelect		(*(s16 *)g_SceneVar3B0A)	/* scene variable: Cerebro_Run runs the player select after the text */
#define g_CerebroScript		(*(PTR(void) *)g_SceneVar3B14)	/* scene variable: the text script Cerebro_Run runs (0: none) */

void CerebroSentinels_RunScene(void)
{
	g_CerebroIntro = 1;
	g_CerebroSelect = 1;
	g_CerebroScript = CODEPTR(Str_CerebroSentinelsText);
	Cerebro_Run();
	g_ProgressFlags |= 2;
}
