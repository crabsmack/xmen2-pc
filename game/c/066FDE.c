/* Credits_RunScene - the scene 'CerebroScreen Credits': the credits text script (Snd_Credits: a
 * Cerebro text script despite its name; Cerebro_Run, no intro, no player select), with sound $A
 * (restarted) unless the game was finished (progress flag $4000); then a new game
 * (Game_ResetPlayers, progress flags 0). */
#include "xmen2.h"

#define g_CerebroIntro		(*(s16 *)g_SceneVar3B0C)	/* scene variable: Cerebro_Run plays the intro first */
#define g_CerebroSelect		(*(s16 *)g_SceneVar3B0A)	/* scene variable: Cerebro_Run runs the player select after the text */
#define g_CerebroScript		(*(PTR(void) *)g_SceneVar3B14)	/* scene variable: the text script Cerebro_Run runs (0: none) */

void Credits_RunScene(void)
{
	g_CerebroIntro = 0;
	g_CerebroSelect = 0;
	g_CerebroScript = CODEPTR(Snd_Credits);
	if (!(g_ProgressFlags & 0x4000)) {
		Snd_Stop(0xA);
		Snd_Play(0xA);
	}
	Cerebro_Run();
	Game_ResetPlayers();
	g_ProgressFlags = 0;
}
