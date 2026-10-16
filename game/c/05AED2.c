/* Boulder_SpawnerTick - the boulder rain (TUSK's arena, Tusk_StateBoulderRain), once a frame:
 * when the delay runs out, drop a BOULDER 80 px above the screen top, 15 frames ahead of a live
 * player in x (both alive: the player the sequence index's bit 0 picks, 1 = player 1), and take
 * the next delay from Tbl_BoulderSpawnDelays (Seq_NextWrap); no boulder while that player (both
 * alive: the higher one) is more than $1C000 above yLimit. Else count the delay down (twice in
 * two-player stepping).
 *
 * yLimit: y (8 fraction bits): TUSK's.
 * The boulder's placement is on the stack with param 0 = 0 (Boulder_Init: start rolling).
 * With no player alive the x is whatever d7 held (the C leaves it unset). */
#include "xmen2.h"

#define g_BoulderSeqIndex	(*(s16 *)g_SceneVar3B04)	/* scene variable: index in Tbl_BoulderSpawnDelays (Boulder_ClassInit clears) */
#define g_BoulderDelay		(*(s16 *)g_SceneVar3B32)	/* scene variable: frames before the next boulder (Boulder_ClassInit clears) */

void Boulder_SpawnerTick(long yLimit)
{
	Placement pl;
	register char alive1, alive2;
	register long limit;
	register long x;

	if (g_BoulderDelay <= 0) {
		short top = g_CameraY - 0x50;

		limit = yLimit + 0x1C000;
		alive1 = IS_ALIVE(&g_Player1);
		alive2 = IS_ALIVE(&g_Player2);
		if (alive1 && alive2) {
			if (MIN(g_Player1.y, g_Player2.y) < limit)
				return;
			if (g_BoulderSeqIndex & 1)
				x = g_Player1.x + g_Player1.vx * 15;
			else
				x = g_Player2.x + g_Player2.vx * 15;
		} else if (alive1) {
			if (limit > g_Player1.y)
				return;
			x = g_Player1.x + g_Player1.vx * 15;
		} else if (alive2) {
			if (limit > g_Player2.y)
				return;
			x = g_Player2.x + g_Player2.vx * 15;
		}
		pl.param[0] = 0;
		(*CLASS(Tbl_Boulder_vtable)->method[2])(0, CLASS(Tbl_Boulder_vtable), x, top << 8, &pl);	/* New */
		Seq_NextWrap((s16 *)CODEPTR(Tbl_BoulderSpawnDelays), &g_BoulderSeqIndex, &g_BoulderDelay);
	} else {
		g_BoulderDelay--;
		if (g_TwoPlayerStep)
			g_BoulderDelay--;
	}
}
