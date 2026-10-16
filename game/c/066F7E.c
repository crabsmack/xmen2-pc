/* LogoAndDemo_RunScene - the scene 'CerebroScreen LogoAndDemo' (power-on, after the attract
 * demos): progress flags reset to 1, the Cerebro intro with its logos and PRESS START
 * (Cerebro_Run, no text, no player select). Start pressed there (progress flag 2) begins a new
 * game (Game_ResetPlayers); else the scene index becomes -1 (set explicitly), so Game_SceneLoop
 * plays the attract demos. */
#include "xmen2.h"

#define g_CerebroIntro		(*(s16 *)g_SceneVar3B0C)	/* scene variable: Cerebro_Run plays the intro first */
#define g_CerebroSelect		(*(s16 *)g_SceneVar3B0A)	/* scene variable: Cerebro_Run runs the player select after the text */
#define g_CerebroScript		(*(PTR(void) *)g_SceneVar3B14)	/* scene variable: the text script Cerebro_Run runs (0: none) */

void LogoAndDemo_RunScene(void)
{
	g_ProgressFlags = 1;
	g_CerebroIntro = 1;
	g_CerebroSelect = 0;
	g_CerebroScript = 0;
	Cerebro_Run();
	if (g_ProgressFlags & 2)
		Game_ResetPlayers();
	else {
		g_SceneIndex = -1;
		g_SceneIndexSet = 1;
	}
}
