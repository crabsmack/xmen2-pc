/* BabyBio_ActMoveVertical - BABYBIO AI-script action: climb from one end of its y range to the
 * other at |speed|: down from the ceiling y, else up; then BabyBio_WaitReachVLimit.
 *
 * self: the object. Returns flags & 8.
 * The home y steps one pixel off the end first, so the wait doesn't end at once. */
#include "xmen2.h"

short BabyBio_ActMoveVertical(BabyBio *self)
{
	short y = self->homeY >> 8;

	self->ax = 0;
	self->vx = 0;
	if (y == self->ceilY) {
		self->homeY += 0x100;
		self->vy = ABS(self->speed);
	} else {
		self->homeY -= 0x100;
		self->vy = -ABS(self->speed);
	}
	SET_STATE(self, (StateFn)BabyBio_WaitReachVLimit);
	return self->flags & 8;
}
