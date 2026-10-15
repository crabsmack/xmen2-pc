/* Cam_StepLookDown - camera step (lookFn) while looking down, and easing back after.
 *
 * self: the camera. Nothing while the look is folded into y (camFlags $10). Released (camFlags 8
 * clear): look moves lookReturn back up; at 0 the step function removes itself. Held, with y
 * not locked and look not negative: look grows by lookStep, but no further than keeps the
 * highest live player lookMinScrY below the screen top. */
#include "xmen2.h"

void Cam_StepLookDown(Camera *self)
{
	register Camera *c = self;

	if (c->camFlags & 0x10)
		return;
	if (!(c->camFlags & 8)) {
		short look = c->look - c->lookReturn;

		if (look < 0) {
			look = 0;
			c->lookFn = 0;
		}
		c->look = look;
	} else if (!(c->camFlags & 2) && c->look >= 0) {
		long topY = 9999999;
		short top, scr, limit;

		if (IS_ALIVE(&g_Player1))
			topY = g_Player1.y;
		if (IS_ALIVE(&g_Player2))
			topY = MIN(topY, g_Player2.y);
		top = topY >> 8;
		scr = top - c->y;
		limit = scr - c->lookMinScrY;
		c->look += c->lookStep;
		if (c->look > limit)
			c->look = limit;
	}
}
