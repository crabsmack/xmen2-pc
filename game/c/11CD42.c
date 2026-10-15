/* BabyBio_WaitReachVLimit - BABYBIO state after BabyBio_ActMoveVertical: once the home y is on
 * its floor or ceiling y, the next step now.
 *
 * self: the object. (It stays the state until then.) */
#include "xmen2.h"

void BabyBio_WaitReachVLimit(BabyBio *self)
{
	short y = self->homeY >> 8;

	if (y == self->floorY || y == self->ceilY) {
		AI_NEXT_STEP(self, self->ai, 0);
	}
}
