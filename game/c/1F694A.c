/* Player_Nearest - the player object nearer to an object, by Manhattan distance.
 *
 * self: the object asking (enemy AI targeting; 65 callers).
 * A player counts only while it is present and alive: if one isn't, the other is returned
 * without measuring. On equal distances, player 2.
 * MPW: out of data registers, it moves x and y to the stack once the distances start (d1 there
 * too) and gives d2 y's register. */
#include "xmen2.h"

XPlayer *Player_Nearest(Actor *self)
{
	long x, y, dx, dy, d1, d2;

	x = self->x;
	y = self->y;
#if defined(XMEN2_NATIVE)
	if (g_opt.players > 2)	/* phase 7: the nearest of the group in play (on ties the later; none: player 2) */
		return P34_Nearest(x, y) ? P34_Nearest(x, y) : &g_Player2;
#endif
	if (!IS_ALIVE(&g_Player1))
		return &g_Player2;
	if (!IS_ALIVE(&g_Player2))
		return &g_Player1;
	dx = g_Player1.x - x;
	dy = g_Player1.y - y;
	d1 = ABS(dx) + ABS(dy);
	dx = g_Player2.x - x;
	dy = g_Player2.y - y;
	d2 = ABS(dx) + ABS(dy);
	if (d2 > d1)
		return &g_Player1;
	return &g_Player2;
}
