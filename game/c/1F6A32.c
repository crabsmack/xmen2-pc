/* Player_NearestLive - the live player nearer to an object, by Manhattan distance; the only live
 * one when the other isn't; 0 when neither is (Player_Nearest instead never returns 0).
 *
 * obj: the object asking.
 * A player is live while present and alive (IS_ALIVE). On equal distances, player 2.
 * MPW: not matched (as Player_Nearest: the ROM copies the position to the stack before each
 * difference and keeps it in d4/d5); verified by shadow tests. */
#include "xmen2.h"

Actor *Player_NearestLive(void *obj)
{
	long x, y, d1, d2;

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
	d1 = ABS(g_Player1.x - x) + ABS(g_Player1.y - y);
	d2 = ABS(g_Player2.x - x) + ABS(g_Player2.y - y);
	if (d2 > d1)
		return (Actor *)&g_Player1;
	return (Actor *)&g_Player2;
}
