/* Brood_StateLimitWait - BROOD state: stand until the countdown (set by Brood_StateWalkToPlayer)
 * runs out, then charge again (Brood_ActCharge).
 *
 * self: the object. Returns flags & 8 when it charges, nothing while it waits. */
#include "xmen2.h"

short Brood_StateLimitWait(Brood *self)
{
	if (self->countdown == 0) {
		SET_STATE(self, Brood_ActCharge);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
