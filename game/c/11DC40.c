/* WallCrawlBio_ActStopFrame0 - WALLCRAWLBIO AI-script action (WallCrawlBio_Script2, on a
 * ceiling): stop, no animation, frame 0, the next step after its wait (waitTime).
 *
 * self: the object. */
#include "xmen2.h"

void WallCrawlBio_ActStopFrame0(WallCrawlBio *self)
{
	STOP_MOVING(self);
	self->anim = 0;
	(*self->cls->method[20])(self, 0);		/* +$56 SetFrame */
	AI_NEXT_STEP(self, self->ai, self->waitTime);
}
