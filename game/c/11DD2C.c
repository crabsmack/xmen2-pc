/* WallCrawlBio_WaitReachVLimit - WALLCRAWLBIO state after WallCrawlBio_ActCrawlVertical: once
 * the home y is on an end of its crawl range (xMin / xMax: y on a wall), the next step now.
 *
 * self: the object. (It stays the state until then.) */
#include "xmen2.h"

void WallCrawlBio_WaitReachVLimit(WallCrawlBio *self)
{
	short y = self->homeY >> 8;

	if (y == self->xMin || y == self->xMax) {
		AI_NEXT_STEP(self, self->ai, 0);
	}
}
