/* CerebroClone_RunScene - the scene 'CerebroScreen Clone': only the player select (Cerebro_Run, no
 * intro, no briefing text). */
#include "xmen2.h"

#define g_CerebroIntro		(*(s16 *)g_SceneVar3B0C)	/* scene variable: Cerebro_Run plays the intro first */
#define g_CerebroSelect		(*(s16 *)g_SceneVar3B0A)	/* scene variable: Cerebro_Run runs the player select after the text */
#define g_CerebroScript		(*(PTR(void) *)g_SceneVar3B14)	/* scene variable: the text script Cerebro_Run runs (0: none) */

void CerebroClone_RunScene(void)
{
	g_CerebroIntro = 0;
	g_CerebroSelect = 1;
	g_CerebroScript = 0;
	Cerebro_Run();
}
