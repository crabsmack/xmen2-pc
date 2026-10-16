/* Player_NearestLive - the live player nearer to an object, by Manhattan distance; the only live
 * one when the other isn't; 0 when neither is (Player_Nearest instead never returns 0).
 *
 * obj: the object asking.
 * A player is live while present and alive (IS_ALIVE). On equal distances, player 2. */
#include "xmen2.h"

Actor *Player_NearestLive(void *obj)
{
	/*DECL*/
	long d1;
	long ox, oy;
	register long dx, dy;
	long x = ((Actor *)obj)->x;
	long y = ((Actor *)obj)->y;
	register long d2;
	/*END*/

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
	ox = x;
	dx = g_Player1.x - ox;
	oy = y;
	dy = g_Player1.y - oy;
	d1 = ABS(dx) + ABS(dy);
	dx = g_Player2.x - ox;
	dy = g_Player2.y - oy;
	d2 = ABS(dx) + ABS(dy);
	if (d2 > d1)
		return (Actor *)&g_Player1;
	return (Actor *)&g_Player2;
}
