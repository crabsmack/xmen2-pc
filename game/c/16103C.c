/* Cam_TrackBoth - the tracking function in two-player mode (Cam_FollowBoth).
 *
 * self: the camera. With one player left alive (g_PlayersAlive 1) it goes back to one-player mode:
 * FollowPlayer (slot +$1E) with the box ($60, $5A, $E0, $C0), g_CamModeSwitched (the offset then
 * absorbs the jump) and the post-update hook. Else x keeps the players' midpoint inside the dead
 * zone's columns (unless camFlags 1), and y puts the lower player at deadBottom, or the upper one
 * at deadTop when that is higher (unless camFlags 2). */
#include "xmen2.h"

void Cam_TrackBoth(Camera *self)
{
	register Camera *c = self;
	register CamFn hook;
	short left = c->deadLeft;
	short right = c->deadRight;
	short top = c->deadTop;
	short bottom = c->deadBottom;
	short x1;
	short x2, y1, y2, cx;

	if (g_PlayersAlive == 1) {
		(*c->cls->method[6])(c, 0x60, 0x5A, 0xE0, 0xC0);	/* FollowPlayer */
		g_CamModeSwitched = 1;
		if (hook = c->postHook)
			hook(c);
		return;
	}
#if defined(XMEN2_NATIVE)
	if (g_opt.players > 2)	/* phase 7: the extents of the group in play */
		P34_Extents(&x1, &x2, &y1, &y2);
	else
#endif
	{
	if (g_Player1.x < g_Player2.x) {
		x1 = g_Player1.x >> 8;
		x2 = g_Player2.x >> 8;
	} else {
		x1 = g_Player2.x >> 8;
		x2 = g_Player1.x >> 8;
	}
	if (g_Player1.y < g_Player2.y) {
		y1 = g_Player1.y >> 8;
		y2 = g_Player2.y >> 8;
	} else {
		y1 = g_Player2.y >> 8;
		y2 = g_Player1.y >> 8;
	}
	}
	cx = (x1 + x2) >> 1;
	if (!(c->camFlags & 1))
		c->x = cx < c->x + left ? cx - left : cx > c->x + right ? cx - right : c->x;
	if (!(c->camFlags & 2)) {
		c->y = y2 - bottom;
		if (y1 < c->y + top)
			c->y = y1 - top;
	}
}
