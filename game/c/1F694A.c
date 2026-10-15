/* Player_Nearest - the player object nearer to an object, by Manhattan distance.
 *
 * self: the object asking (enemy AI targeting; 65 callers).
 * A player counts only while it is present and alive: if one isn't, the other is returned
 * without measuring. On equal distances, player 2. */
#include "xmen2.h"

XPlayer *Player_Nearest(Actor *self)
{
	long x, y, d1, d2;

	x = self->x;
	y = self->y;
	if (!IS_ALIVE(&g_Player1))
		return &g_Player2;
	if (!IS_ALIVE(&g_Player2))
		return &g_Player1;
	d1 = ABS(g_Player1.x - x) + ABS(g_Player1.y - y);
	d2 = ABS(g_Player2.x - x) + ABS(g_Player2.y - y);
	if (d2 > d1)
		return &g_Player1;
	return &g_Player2;
}
