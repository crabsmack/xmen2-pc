/* SentinelElevator_StateWaitSwitch - SENTINELELEVATOR state: wait for its switch's flag bit 0,
 * then go up (SentinelElevator_StateGoUp from the next update).
 *
 * self: the lift.
 * Returns self->flags & 8 when it goes, nothing while it waits. */
#include "xmen2.h"

short SentinelElevator_StateWaitSwitch(SentinelElevator *self)
{
	register s16 *flags = self->switchFlags;

	if (*flags & 1) {
		SET_STATE(self, SentinelElevator_StateGoUp);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
