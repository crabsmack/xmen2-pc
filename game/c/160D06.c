/* Cam_StepLookUp - camera step (lookFn) while looking up, and easing back after.
 *
 * self: the camera. Nothing while the look is folded into y (camFlags $10). Released (camFlags 4
 * clear): look moves lookReturn back down; at 0 the step function removes itself. Held, with y
 * not locked and look not positive: look falls by lookStep, but no further than keeps the
 * lowest live player at most lookMaxScrY below the screen top. */
#include "xmen2.h"

void Cam_StepLookUp(Camera *self)
{
	register Camera *c = self;

	if (c->camFlags & 0x10)
		return;
	if (!(c->camFlags & 4)) {
		short look = c->look + c->lookReturn;

		if (look > 0) {
			look = 0;
			c->lookFn = 0;
		}
		c->look = look;
	} else if (!(c->camFlags & 2) && c->look <= 0) {
		long bottomY = -9999999;
		short bottom, scr, limit;

		if (IS_ALIVE(&g_Player1))
			bottomY = g_Player1.y;
		if (IS_ALIVE(&g_Player2))
			bottomY = MAX(bottomY, g_Player2.y);
		bottom = bottomY >> 8;
		scr = bottom - c->y;
		limit = scr - c->lookMaxScrY;
		c->look -= c->lookStep;
		if (c->look < limit)
			c->look = limit;
	}
}
