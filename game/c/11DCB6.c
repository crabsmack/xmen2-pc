/* WallCrawlBio_ActStopFrame11 - WALLCRAWLBIO AI-script action (WallCrawlBio_Script1, on a
 * wall): stop, no animation, frame 11, the next step after its wait (waitTime).
 *
 * self: the object. */
#include "xmen2.h"

void WallCrawlBio_ActStopFrame11(WallCrawlBio *self)
{
	STOP_MOVING(self);
	self->anim = 0;
	(*self->cls->method[20])(self, 11);		/* +$56 SetFrame */
	AI_NEXT_STEP(self, self->ai, self->waitTime);
}
