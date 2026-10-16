/* BabyBio_WaitStopOrLimit - BABYBIO state: once it has no x speed or the home x is on an end of
 * its range, stop and take the next step on the next update.
 *
 * self: the object. Returns flags & 8 while it waits (nothing in particular after).
 * Never runs in the corpus. */
#include "xmen2.h"

short BabyBio_WaitStopOrLimit(BabyBio *self)
{
	short x = self->homeX >> 8;

	if (!self->vx || x == self->xMin || x == self->xMax) {
		STOP_MOVING(self);
		AI_NEXT_STEP(self, self->ai, 0);
	} else {
		SET_STATE(self, BabyBio_WaitStopOrLimit);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
