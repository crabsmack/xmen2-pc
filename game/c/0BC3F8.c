/* SavageBio_ActWait30 - SAVAGEBIGBIO script action: go on with the script in $1E frames.
 *
 * self: the boss (a step of its AI script; never runs in the corpus). */
#include "xmen2.h"

void SavageBio_ActWait30(SavageBigBio *self)
{
	AI_NEXT_STEP(self, self->ai, 0x1E);
}
