/* Game_ResetPlayers - a new game: both players out (no character, no class, flags 0, health 0),
 * 9 lives each (NATIVE: the lives option's count), and the first stage (g_ScnSiberiaB1) as the next scene. */
#include "xmen2.h"

void Game_ResetPlayers(void)
{
	g_P1Health = 0;
	g_P1CharIndex = -1;
	g_Player1.cls = 0;
	g_Player1.flags = 0;
	g_P1Lives = START_LIVES;
	g_P2Health = 0;
	g_P2CharIndex = -1;
	g_Player2.cls = 0;
	g_Player2.flags = 0;
	g_P2Lives = START_LIVES;
#if defined(XMEN2_NATIVE)
	if (g_opt.players > 2)
		P34_ResetPlayers();	/* phase 7: players 3 and 4 the same */
	g_SceneIndex = plat_debug_first_scene(g_ScnSiberiaB1);	/* XMEN2_DEBUG_SCENE: a development test */
#define CEREBRO_SCREEN(n)	(g_SceneTable[n].name[0] == 'C' && g_SceneTable[n].name[1] == 'e')	/* "CerebroScreen ..." */
	if (g_SceneIndex > 0 && !CEREBRO_SCREEN(g_SceneIndex) && !CEREBRO_SCREEN(g_SceneIndex - 1)) {
		/* ... at a stage with no select before it (SentinelsB2, after SentinelsA1; a "CerebroScreen"
		   runs one): the select first, then the stage, as after a death */
		g_ResumeSceneIndex = g_SceneIndex;
		g_SceneIndex = g_ScnPlayerSelect;
	}
#else
	g_SceneIndex = g_ScnSiberiaB1;
#endif
	g_SceneIndexSet = 1;
}
