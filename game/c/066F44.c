/* PlayerSelect_RunScene - the scene 'CerebroScreen PlayerSelect' (between stages, and after a
 * death): the player select (Cerebro_Run, no intro, no text). After a death (g_ResumeSceneIndex
 * >= 0, saved by Game_SceneLoop) the game goes back to the scene the player died in: g_SceneIndex
 * is set from it before, marked as set explicitly after, and the saved index cleared (-1). */
#include "xmen2.h"

#define g_CerebroIntro		(*(s16 *)g_SceneVar3B0C)	/* scene variable: Cerebro_Run plays the intro first */
#define g_CerebroSelect		(*(s16 *)g_SceneVar3B0A)	/* scene variable: Cerebro_Run runs the player select after the text */
#define g_CerebroScript		(*(PTR(void) *)g_SceneVar3B14)	/* scene variable: the text script Cerebro_Run runs (0: none) */

void PlayerSelect_RunScene(void)
{
	g_CerebroIntro = 0;
	g_CerebroSelect = 1;
	g_CerebroScript = 0;
	if (g_ResumeSceneIndex >= 0)
		g_SceneIndex = g_ResumeSceneIndex;
	Cerebro_Run();
	if (g_ResumeSceneIndex >= 0) {
		g_SceneIndexSet = 1;
		g_ResumeSceneIndex = -1;
	}
}
