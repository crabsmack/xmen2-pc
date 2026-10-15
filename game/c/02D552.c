/* Exodus_StateRise - EXODUS state: fly up (vy -200, ay -10; x and y move) until above the saved
 * height (Exodus_StateRiseUntil).
 *
 * self: the boss. Returns self->flags & 8.
 * vx = dx / steps as in Exodus_StateSwoopStart, towards the saved point: dx and dy are only what
 * is left of the pixel (the target is where it is), so vx comes out 0 or a small drift, and a
 * 0 step count would divide by zero. */
#include "xmen2.h"

short Exodus_StateRise(register Exodus *self)
{
	register long dx, steps, dy;

	dx = (self->targetX << 8) - self->x;
	dy = (self->targetY << 8) - self->y;
	self->vy = -200;
	self->ay = -10;
	steps = ABS(dy) / ABS(self->vy);
	self->vx = (short)(dx / steps);
	self->moveFlags |= 2;
	self->moveFlags |= 1;
	SET_STATE(self, Exodus_StateRiseUntil);
	return self->flags & 8;
}
