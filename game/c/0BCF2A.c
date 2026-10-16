/* SavageMediumBio_StateWalk - SAVAGEMEDIUMBIO state: walk until the countdown (SavageMediumBio_ActWalk)
 * has run out, then go on with the script on the next update.
 *
 * self: the object. Returns self->flags & 8 while it walks, nothing in particular after.
 * Never runs in the corpus. */
#include "xmen2.h"

short SavageMediumBio_StateWalk(register SavageMediumBio *self)
{
	if (self->countdown <= 0) {
		AI_NEXT_STEP(self, self->ai, 0);
	} else {
		SET_STATE(self, SavageMediumBio_StateWalk);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
