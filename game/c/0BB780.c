/* SavageBio_StateExtend10 - SAVAGEBIGBIO state: grow the tentacle one segment per frame to 10
 * segments, then go on with the script.
 *
 * self: the boss (after SavageBio_ActExtendShort).
 * Returns self->flags & 8 while growing; nothing in particular when the script goes on. */
#include "xmen2.h"

short SavageBio_StateExtend10(SavageBigBio *self)
{
	if (self->tentacleLen >= 10) {
		AI_NEXT_STEP(self, self->ai, 0);
	} else {
		self->tentacleLen++;
		SET_STATE(self, SavageBio_StateExtend10);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
