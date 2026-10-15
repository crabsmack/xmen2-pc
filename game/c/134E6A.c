/* FlyBrood_WaitPathDone - FLYBROOD state of the type-4 flyer (FlyBrood_Init): wait until its
 * homing path has ended, then its AI script from the first step now.
 *
 * self: the object. Returns flags & 8 while it waits (nothing in particular after).
 * Never runs in the corpus. */
#include "xmen2.h"

short FlyBrood_WaitPathDone(FlyBrood *self)
{
	if (!self->path) {
		AI_FIRST_STEP(self, self->ai, 0);
	} else {
		SET_STATE(self, FlyBrood_WaitPathDone);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
