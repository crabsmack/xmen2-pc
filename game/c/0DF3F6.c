/* SentinelElevator_StateGoUp - SENTINELELEVATOR state: start rising (flags6 bit $4000 cleared,
 * vy -$100); SentinelElevator_StateRising from the next update.
 *
 * self: the lift.
 * Returns self->flags & 8. */
#include "xmen2.h"

short SentinelElevator_StateGoUp(register SentinelElevator *self)
{
	self->flags6 &= ~0x4000;
	self->vy = -0x100;
	SET_STATE(self, SentinelElevator_StateRising);
	return self->flags & 8;
}
