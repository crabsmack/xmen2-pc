/* BrainRocket_Move - BRAINROCKET Move (class slot +$4E; the parent's isn't called): per step, the
 * position moves by the velocity and the velocity by the acceleration; no speed limits, no floor.
 *
 * self: the rocket.  steps: steps this frame (1, 2 in two-player step mode; at least 1). */
#include "xmen2.h"

void BrainRocket_Move(register BrainRocket *self, register short steps)
{
	do {
		self->x += self->vx;
		self->y += self->vy;
		self->vx += self->ax;
		self->vy += self->ay;
	} while (--steps);
}
