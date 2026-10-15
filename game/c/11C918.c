/* BabyBio_ActWaitParam - BABYBIO AI-script action: next step after its wait (delay, placement
 * param 3).
 *
 * self: the object. Never runs in the corpus. */
#include "xmen2.h"

void BabyBio_ActWaitParam(BabyBio *self)
{
	AI_NEXT_STEP(self, self->ai, self->delay);
}
