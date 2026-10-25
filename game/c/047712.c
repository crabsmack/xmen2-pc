/* BaniMazaA2_PlacePlayers - put the players at the Apocalypse room's start (Players_PlaceAtStart,
 * all arguments 0), then each player present and alive gets sprite flag bit 2 and its draw-list
 * node goes to priority 1 at its layer (DrawList_SetPriority; flags6 bit 3 passed on): the
 * opposite of BaniMazaA1_PlacePlayers. */
#include "xmen2.h"

void BaniMazaA2_PlacePlayers(void)
{
	Players_PlaceAtStart(0, 0, 0, 0);
	if (IS_ALIVE(&g_Player1)) {
		g_Player1.spr.flags |= 4;
		DrawList_SetPriority(g_Player1.drawNode, 1, g_Player1.layer, g_Player1.flags6 & 8);
	}
	if (IS_ALIVE(&g_Player2)) {
		g_Player2.spr.flags |= 4;
		DrawList_SetPriority(g_Player2.drawNode, 1, g_Player2.layer, g_Player2.flags6 & 8);
	}
#if defined(XMEN2_NATIVE)
	P34_SetLayer(1, 1);	/* players 3-4 in play too (R11) */
#endif
}
