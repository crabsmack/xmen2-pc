/* SavageJungle_PlacePlayers - put the players at the jungle's start (Players_PlaceAtStart $4000,
 * $4000, $4010, $4010), then each player present and alive loses sprite flag bit 2 and its
 * draw-list node goes to priority 0 at its layer (DrawList_SetPriority; flags6 bit 3 passed on),
 * as BaniMazaA1_PlacePlayers. */
#include "xmen2.h"

void SavageJungle_PlacePlayers(void)
{
	Players_PlaceAtStart(0x4000, 0x4000, 0x4010, 0x4010);
	if (IS_ALIVE(&g_Player1)) {
		g_Player1.spr.flags &= ~4;
		DrawList_SetPriority(g_Player1.drawNode, 0, g_Player1.layer, g_Player1.flags6 & 8);
	}
	if (IS_ALIVE(&g_Player2)) {
		g_Player2.spr.flags &= ~4;
		DrawList_SetPriority(g_Player2.drawNode, 0, g_Player2.layer, g_Player2.flags6 & 8);
	}
#if defined(XMEN2_NATIVE)
	P34_SetLayer(0, 1);	/* players 3-4 in play too (R11) */
#endif
}
