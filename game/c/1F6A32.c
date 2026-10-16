/* Player_NearestLive - the live player nearer to an object, by Manhattan distance; the only live
 * one when the other isn't; 0 when neither is (Player_Nearest instead never returns 0).
 *
 * obj: the object asking.
 * A player is live while present and alive (IS_ALIVE). On equal distances, player 2. The second
 * test of player 1 after player 2 is found dead can't fail (player 1 was just found alive). */
#include "xmen2.h"

Actor *Player_NearestLive(void *obj)
{
	long x, y, dx, dy, d1, d2;

	x = ((Actor *)obj)->x;
	y = ((Actor *)obj)->y;
	if (!IS_ALIVE(&g_Player1)) {
		if (!IS_ALIVE(&g_Player2))
			return 0;
		return (Actor *)&g_Player2;
	}
	if (!IS_ALIVE(&g_Player2)) {
		if (!IS_ALIVE(&g_Player1))
			return 0;
		return (Actor *)&g_Player1;
	}
	dx = g_Player1.x - x;
	dy = g_Player1.y - y;
	d1 = ABS(dx) + ABS(dy);
	dx = g_Player2.x - x;
	dy = g_Player2.y - y;
	d2 = ABS(dx) + ABS(dy);
	if (d2 > d1)
		return (Actor *)&g_Player1;
	return (Actor *)&g_Player2;
}
