/* BioBalls_WaitPathDone - BIOBALLS state (BioBalls_StartPath): wait until the balls have
 * reached their path (BioBalls_Move clears it), then the next step of the AI script on the next
 * update.
 *
 * self: the object.
 * Returns flags bit 3 while waiting; nothing in particular after. */
#include "xmen2.h"

short BioBalls_WaitPathDone(BioBalls *self)
{
	if (self->path) {
		SET_STATE(self, BioBalls_WaitPathDone);
		return self->flags & 8;
	}
	AI_NEXT_STEP(self, self->ai, 0);
	NO_RETURN_VALUE
}
