/* WallCrawlBio_WaitReachHLimit - WALLCRAWLBIO state after WallCrawlBio_ActCrawlHorizontal: once
 * the home x is on an end of its crawl range (xMin / xMax), the next step on the next update.
 *
 * self: the object. (It stays the state until then.) */
#include "xmen2.h"

void WallCrawlBio_WaitReachHLimit(WallCrawlBio *self)
{
	short x = self->homeX >> 8;

	if (x == self->xMin || x == self->xMax) {
		AI_NEXT_STEP(self, self->ai, 0);
	}
}
