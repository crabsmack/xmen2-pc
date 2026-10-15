/* BigAcolyte_StateWaitYNext - BIGACOLYTE state: check every 4 frames whether it has reached its
 * target height; then the next step of its AI script.
 *
 * self: the boss (never runs in the corpus).
 * Returns self->flags & 8 while waiting, nothing when the script goes on. */
#include "xmen2.h"

short BigAcolyte_StateWaitYNext(register BigAcolyte *self)
{
	if (self->arrivedY) {
		AI_NEXT_STEP(self, self->ai, 0);
	} else {
		SET_STATE_WAIT(self, BigAcolyte_StateWaitYNext, 4);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
