/* Game_ResetPlayers - a new game: both players out (no character, no class, flags 0, health 0),
 * 9 lives each, and the first stage (g_ScnSiberiaB1) as the next scene. */
#include "xmen2.h"

void Game_ResetPlayers(void)
{
	g_P1Health = 0;
	g_P1CharIndex = -1;
	g_Player1.cls = 0;
	g_Player1.flags = 0;
	g_P1Lives = 9;
	g_P2Health = 0;
	g_P2CharIndex = -1;
	g_Player2.cls = 0;
	g_Player2.flags = 0;
	g_P2Lives = 9;
#if defined(XMEN2_NATIVE)
	g_SceneIndex = plat_debug_first_scene(g_ScnSiberiaB1);	/* XMEN2_DEBUG_SCENE: a development test */
#else
	g_SceneIndex = g_ScnSiberiaB1;
#endif
	g_SceneIndexSet = 1;
}
