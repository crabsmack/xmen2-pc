/* SavageBio_ActStop60 - SAVAGEBIGBIO script action: stop, go on with the script in $3C frames.
 *
 * self: the boss (a step of its AI script). */
#include "xmen2.h"

void SavageBio_ActStop60(SavageBigBio *self)
{
	STOP_MOVING(self);
	AI_NEXT_STEP(self, self->ai, 0x3C);
}
