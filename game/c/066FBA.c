/* FinalText_RunScene - the scene 'CerebroScreen FinalText' (after the last stage): the ending text
 * (Tbl_FinalTextScript; Cerebro_Run, no intro, no player select); then progress flag $4000 (game
 * finished) is set. */
#include "xmen2.h"

#define g_CerebroIntro		(*(s16 *)g_SceneVar3B0C)	/* scene variable: Cerebro_Run plays the intro first */
#define g_CerebroSelect		(*(s16 *)g_SceneVar3B0A)	/* scene variable: Cerebro_Run runs the player select after the text */
#define g_CerebroScript		(*(PTR(void) *)g_SceneVar3B14)	/* scene variable: the text script Cerebro_Run runs (0: none) */

void FinalText_RunScene(void)
{
	g_CerebroIntro = 0;
	g_CerebroSelect = 0;
	g_CerebroScript = CODEPTR(Tbl_FinalTextScript);
	Cerebro_Run();
	g_ProgressFlags |= 0x4000;
}
