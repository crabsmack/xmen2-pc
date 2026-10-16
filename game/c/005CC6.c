/* Game_SceneLoop - the game: scenes one after another (g_SceneTable from g_SceneIndex: Cerebro
 * screens, the player select, the stage parts), with the players' health, lives and game over;
 * past the scene list (index -1), the attract demos in turn. Never returns.
 *
 * Before each scene its load hooks (Scene_PreLoadReset, Scene_PostLoadVram) are set; after it
 * Cyclops_ResetBeams, and the health of each playing player is kept (g_P1Health...). A player out
 * of health spends a life ($700 health), or with no lives left is out of the game (no
 * character, no class). Unless the scene chose the next one (g_SceneIndexSet): with a player
 * still standing, the next scene (a new game past the credits); both down, the player select if
 * one still has health (back to this scene after it: g_ResumeSceneIndex), else the failure
 * screen.
 * Demos (cases of the g_AttractDemoNum switch; the fragments Game_AttractDemoNightcrawler,
 * Game_AttractDemoCyclops and the shared tail Game_PlayAttractDemo): player 1 as the demo's
 * character with health $900, its recorded input (g_DemoInput), sound muted; a skipped demo sets
 * g_ProgressFlags bit 1; then a new game. */
#include "xmen2.h"

void Game_SceneLoop(void)
{
	register void *input;		/* the demo's recorded input */
	register Class *cls;		/* the demo's character */
	register long scene;		/* the demo's scene */
	register char p1Out, p2Out;	/* no health left after the scene */

	g_Unk38B0 = 0;
	g_DemoActive = 0;
	g_StartEndsScene = 0;
	g_SegFlag38C4 = 0;
	g_Unk38AC = 3;
	g_Unk3B00 = 0;
	Scene_RegisterAll();
	g_ResumeSceneIndex = -1;
	Game_ResetPlayers();
	for (;;) {
		if (g_SceneTable[g_SceneIndex].run) {
			g_ScenePreLoadHook = FN_DATA(Scene_PreLoadReset);
			g_ScenePostLoadHook = FN_DATA(Scene_PostLoadVram);
			(*g_SceneTable[g_SceneIndex].run)();
			Cyclops_ResetBeams();
		}
		if (g_P1CharIndex != -1)
			g_P1Health = g_Player1.health;
		if (g_P2CharIndex != -1)
			g_P2Health = g_Player2.health;
		p1Out = g_P1Health > 0 ? 0 : 1;
		p2Out = g_P2Health > 0 ? 0 : 1;
		if (p1Out && g_P1CharIndex != -1) {
			if (g_P1Lives) {
				--g_P1Lives;
				g_P1Health = 0x700;
			} else {
				g_P1Health = 0;
				g_P1CharIndex = -1;
				g_Player1.cls = 0;
				g_Player1.flags = 0;
			}
		}
		if (p2Out && g_P2CharIndex != -1) {
			if (g_P2Lives) {
				--g_P2Lives;
				g_P2Health = 0x700;
			} else {
				g_P2Health = 0;
				g_P2CharIndex = -1;
				g_Player2.cls = 0;
				g_Player2.flags = 0;
			}
		}
		if (!g_SceneIndexSet) {
			if (!p1Out || !p2Out) {
				++g_SceneIndex;
				if (g_ScnCredits < g_SceneIndex)
					Game_ResetPlayers();
			} else if (g_P1Health || g_P2Health) {
				g_ResumeSceneIndex = g_SceneIndex;
				g_SceneIndex = g_ScnPlayerSelect;
			} else
				g_SceneIndex = g_ScnFailure;
		} else if (g_SceneIndex == -1) {
			switch (g_AttractDemoNum) {
			default:		/* Game_AttractDemoNightcrawler */
				g_AttractDemoNum = 0;
			case 0:
				scene = g_ScnBaniMazaA1;
				input = CODEPTR(Tbl_DemoInputNightcrawler);
				cls = CLASS(Tbl_Nightcrawler_vtable);
				break;
			case 1:			/* Game_AttractDemoCyclops */
				scene = g_ScnSentinelsB1;
				input = CODEPTR(Tbl_DemoInputCyclops);
				cls = CLASS(Tbl_Cyclops_vtable);
				break;
			case 2:
				scene = g_ScnAvalonB2;
				input = CODEPTR(Tbl_DemoInputPsylocke);
				cls = CLASS(Tbl_Psylocke_vtable);
				break;
			case 3:
				scene = g_ScnSentinelsD1;
				input = CODEPTR(Tbl_DemoInputBeast);
				cls = CLASS(Tbl_Beast_vtable);
				break;
			}
			/* Game_PlayAttractDemo */
			++g_AttractDemoNum;
			g_Player1.cls = cls;
			g_P1Health = 0x900;
			g_DemoInput = input;
			g_DemoActive = 1;
			g_SceneIndex = scene;
			g_ScenePreLoadHook = FN_DATA(Scene_PreLoadReset);
			g_ScenePostLoadHook = FN_DATA(Scene_PostLoadVram);
			g_SndMute = 1;
			(*g_SceneTable[scene].run)();
			g_SndMute = 0;
			Cyclops_ResetBeams();
			if (g_DemoSkipped)
				g_ProgressFlags |= 2;
			Game_ResetPlayers();
		}
		g_SceneIndexSet = 0;
	}
}
