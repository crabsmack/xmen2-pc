/* BigAcolyte_StateWaitArrive - BIGACOLYTE state: wait until it has arrived on both axes, then
 * the next step of its AI script in 120 frames.
 *
 * self: the boss. Returns self->flags & 8 while waiting, nothing when the script goes on. */
#include "xmen2.h"

short BigAcolyte_StateWaitArrive(register BigAcolyte *self)
{
	if (self->arrivedY && self->arrivedX) {
		AI_NEXT_STEP(self, self->ai, 120);
	} else {
		SET_STATE(self, BigAcolyte_StateWaitArrive);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
