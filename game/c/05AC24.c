/* BaniMazaB2_PlacePlayers - put the players at BaniMazaB2's (Exterior) start (Players_PlaceAtStart,
 * all arguments 0), each present one with its sprite's priority bit (spr.flags bit 2) set. */
#include "xmen2.h"

void BaniMazaB2_PlacePlayers(void)
{
	Players_PlaceAtStart(0, 0, 0, 0);
	if (IS_ALIVE(&g_Player1))
		g_Player1.spr.flags |= 4;
	if (IS_ALIVE(&g_Player2))
		g_Player2.spr.flags |= 4;
}
