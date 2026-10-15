/* Exodus_StateRiseUntil - EXODUS state: once above the saved height (y < targetY), end the rise
 * (Exodus_StateRiseEnd).
 *
 * self: the boss. Returns self->flags & 8 when it switches, no value while waiting. */
#include "xmen2.h"

short Exodus_StateRiseUntil(register Exodus *self)
{
	if (self->targetY << 8 > self->y) {
		SET_STATE(self, Exodus_StateRiseEnd);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
