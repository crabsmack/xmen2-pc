/* BabyBio_WaitReachHLimit - BABYBIO state after BabyBio_ActMoveHorizontal: once the home x is on
 * an end of its patrol range, the next step now.
 *
 * self: the object. (It stays the state until then.) Never runs in the corpus. */
#include "xmen2.h"

void BabyBio_WaitReachHLimit(BabyBio *self)
{
	short x = self->homeX >> 8;

	if (x == self->xMin || x == self->xMax) {
		AI_NEXT_STEP(self, self->ai, 0);
	}
}
