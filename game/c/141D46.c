/* TankLeft_Move - TANKLEFT Move (class slot +$4E; the parent's isn't called): position +=
 * velocity, velocity += acceleration, twice when steps > 1. Unlike TANK's, no track limits: it
 * drives in and away.
 *
 * self: the object.  steps: frames to move (1, or 2 in the two-player step mode). */
#include "xmen2.h"

void TankLeft_Move(register TankLeft *self, short steps)
{
	self->x += self->vx;
	self->y += self->vy;
	self->vx += self->ax;
	self->vy += self->ay;
	if (steps > 1) {
		self->x += self->vx;
		self->y += self->vy;
		self->vx += self->ax;
		self->vy += self->ay;
	}
}
