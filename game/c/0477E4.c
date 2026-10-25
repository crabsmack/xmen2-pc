/* BaniMazaA2_DrawHook - the Apocalypse room's draw hook (g_SceneDrawHook): the draw list
 * (DrawList_DrawAll), then the floor shadow of each player present and alive
 * (ApocBomb_DrawShadow). */
#include "xmen2.h"

void BaniMazaA2_DrawHook(void)
{
	DrawList_DrawAll();
	if (IS_ALIVE(&g_Player1))
		ApocBomb_DrawShadow((Actor *)&g_Player1);
	if (IS_ALIVE(&g_Player2))
		ApocBomb_DrawShadow((Actor *)&g_Player2);
#if defined(XMEN2_NATIVE)
	for (int k = 3; k <= g_opt.players; k++)	/* players 3-4 in play too (R11); in the main sprite table */
		if (IS_ALIVE(PLAYER34(k)))
			ApocBomb_DrawShadow((Actor *)PLAYER34(k));
#endif
}
