/* SavageBio_ActStop30 - SAVAGEBIGBIO script action: stop, go on with the script in $1E frames.
 *
 * self: the boss (a step of its AI script). */
#include "xmen2.h"

void SavageBio_ActStop30(SavageBigBio *self)
{
	STOP_MOVING(self);
	AI_NEXT_STEP(self, self->ai, 0x1E);
}
