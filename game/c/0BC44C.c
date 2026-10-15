/* SavageBio_ActWait5 - SAVAGEBIGBIO script action: go on with the script in 5 frames.
 *
 * self: the boss (a step of its AI script; never runs in the corpus). */
#include "xmen2.h"

void SavageBio_ActWait5(SavageBigBio *self)
{
	AI_NEXT_STEP(self, self->ai, 5);
}
