/* BabyBio_ActStopWait60 - BABYBIO AI-script action: stop, next step in 60 frames.
 *
 * self: the object. */
#include "xmen2.h"

void BabyBio_ActStopWait60(BabyBio *self)
{
	STOP_MOVING(self);
	AI_NEXT_STEP(self, self->ai, 60);
}
