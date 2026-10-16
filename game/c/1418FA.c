/* Tank_Move - TANK Move (class slot +$4E; the parent's isn't called): position += velocity,
 * velocity += acceleration, twice when steps > 1; then x is kept inside the track
 * (trackLeft..trackRight), the x motion stopped at an end.
 *
 * self: the object.  steps: frames to move (1, or 2 in the two-player step mode).
 * Never runs in the corpus. */
#include "xmen2.h"

void Tank_Move(register Tank *self, short steps)
{
	register short x;

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
	x = self->x >> 8;
	if (x < self->trackLeft) {
		self->x = (long)self->trackLeft << 8;
		self->ax = 0;
		self->vx = 0;
	} else if (x > self->trackRight) {
		self->x = (long)self->trackRight << 8;
		self->ax = 0;
		self->vx = 0;
	}
}
