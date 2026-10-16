/* Player2_MakeHostile - let the players hit each other (the pause cheat, Tbl_CheatCodeFriendlyFire;
 * Players_PlaceAtStart): with both players alive, player 2 leaves the player hit lists for the
 * enemy ones and takes palette 3.
 */
#include "xmen2.h"

void Player2_MakeHostile(void)
{
	if (IS_ALIVE(&g_Player1) && IS_ALIVE(&g_Player2)) {
		List_Unlink(g_Player2.attackNode, &g_HitListPlayerAtk);
		g_Player2.attackNode = 0;
		List_Unlink(g_Player2.bodyNode, &g_HitListPlayerBody);
		g_Player2.bodyNode = 0;
		g_Player2.attackNode = HitList_Add(&g_Player2, &g_HitListEnemyAtk);
		g_Player2.bodyNode = HitList_Add(&g_Player2, &g_HitListEnemyBody);
		g_Player2.spr.flags = g_Player2.spr.flags & ~3 | 3;
	}
}
