/* BabyBio_WaitCrawlLimit - BABYBIO state after BabyBio_ActCrawl: once the home x is on an end of
 * its range, the next step on the next update.
 *
 * self: the object. Returns flags & 8 while it waits (nothing in particular after). */
#include "xmen2.h"

short BabyBio_WaitCrawlLimit(BabyBio *self)
{
	short x = self->homeX >> 8;

	if (x == self->xMin || x == self->xMax) {
		AI_NEXT_STEP(self, self->ai, 0);
	} else {
		SET_STATE(self, BabyBio_WaitCrawlLimit);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
