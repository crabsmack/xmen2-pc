/* BaniMazaA1_PlacePlayers - put the players at the Moving Blocks' start (Players_PlaceAtStart
 * $38400, $7000, $38410, $38410), then each player present and alive loses sprite flag bit 2
 * and its draw-list node goes to priority 0 at its layer (DrawList_SetPriority; flags6 bit 3
 * passed on). BaniMazaA2_PlacePlayers does the opposite. */
#include "xmen2.h"

void BaniMazaA1_PlacePlayers(void)
{
	Players_PlaceAtStart(0x38400, 0x7000, 0x38410, 0x38410);
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
