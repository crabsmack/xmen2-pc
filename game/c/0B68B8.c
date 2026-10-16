/* BrainBabyBio_StateStart - BRAINBABYBIO's first state (set by BrainBabyBio_Init): its AI script
 * from the first step in 10 frames (no script: the default state).
 *
 * self: the object. */
#include "xmen2.h"

void BrainBabyBio_StateStart(BrainBabyBio *self)
{
	AI_FIRST_STEP(self, self->ai, 10);
}
