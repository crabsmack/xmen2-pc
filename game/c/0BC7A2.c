/* SavageBioBalls_StateWait - BIOBALLS state (the SAVAGEBIGBIO's balls form): wait until the
 * countdown has run out (SavageBioBalls_ActMotion), then go on with the AI script now.
 *
 * self: the balls. Returns self->flags & 8 while it waits, nothing in particular after. */
#include "xmen2.h"

short SavageBioBalls_StateWait(register BioBalls *self)
{
	if (self->countdown <= 0) {
		AI_NEXT_STEP(self, self->ai, 0);
	} else {
		SET_STATE(self, SavageBioBalls_StateWait);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
