/* CerebroAvalon_RunScene - the scene 'CerebroScreen Avalon': the Avalon briefing
 * (Str_CerebroAvalonText) and the player select (Cerebro_Run, no intro). */
#include "xmen2.h"

#define g_CerebroIntro		(*(s16 *)g_SceneVar3B0C)	/* scene variable: Cerebro_Run plays the intro first */
#define g_CerebroSelect		(*(s16 *)g_SceneVar3B0A)	/* scene variable: Cerebro_Run runs the player select after the text */
#define g_CerebroScript		(*(PTR(void) *)g_SceneVar3B14)	/* scene variable: the text script Cerebro_Run runs (0: none) */

void CerebroAvalon_RunScene(void)
{
	g_CerebroIntro = 0;
	g_CerebroSelect = 1;
	g_CerebroScript = CODEPTR(Str_CerebroAvalonText);
	Cerebro_Run();
}
