/* BabyBio_ActStartWalk - BABYBIO AI-script action: walk at |speed| away from the end of its range
 * it stands on (right from xMin, else left), turned that way, then
 * BabyBio_ActWalkUntilPlayerAhead. With no range (xMin = xMax) it just stops.
 *
 * self: the object. Returns flags & 8.
 * The home x steps one pixel off the end first, so the walk doesn't end at once. */
#include "xmen2.h"

short BabyBio_ActStartWalk(BabyBio *self)
{
	short x = self->homeX >> 8;

	if (self->xMin == self->xMax)
		STOP_MOVING(self);
	else {
		self->vy = 0;
		self->ay = 0;
		if (x == self->xMin) {
			self->spr.flags &= ~0x10;
			self->homeX += 0x100;
			self->vx = ABS(self->speed);
		} else {
			self->spr.flags |= 0x10;
			self->homeX -= 0x100;
			self->vx = -ABS(self->speed);
		}
	}
	SET_STATE(self, BabyBio_ActWalkUntilPlayerAhead);
	return self->flags & 8;
}
