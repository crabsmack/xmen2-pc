/* ChildShip_StateEnter - CHILDSHIP's first state: fly at $100 (cruise y $F0) to the middle of
 * the view (g_CameraX + $A0); within 10 px it stops and starts its AI script from the first step
 * (from the next update; no script: the default state) and returns 0.
 *
 * self: the object. Returns 0 on arriving, else self->flags & 8. */
#include "xmen2.h"

short ChildShip_StateEnter(ChildShip *self)
{
	short x = self->x >> 8;
	short target = g_CameraX + 0xA0;

	if (ABS(target - x) < 10) {
		STOP_MOVING(self);
		AI_FIRST_STEP(self, self->ai, 0);
		return 0;
	}
	if (x < target)
		self->vx = 0x100;
	else
		self->vx = -0x100;
	self->homeY = 0xF000;
	SET_STATE(self, ChildShip_StateEnter);
	return self->flags & 8;
}
