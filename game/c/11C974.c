/* BabyBio_ActStopWaitParam - BABYBIO AI-script action: stop, next step after its wait (delay,
 * placement param 3).
 *
 * self: the object. */
#include "xmen2.h"

void BabyBio_ActStopWaitParam(BabyBio *self)
{
	STOP_MOVING(self);
	AI_NEXT_STEP(self, self->ai, self->delay);
}
