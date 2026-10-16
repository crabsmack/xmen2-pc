/* SentinelElevator_StateGoDown - SENTINELELEVATOR state: start sinking (flags6 bit $4000
 * cleared, vy $100); SentinelElevator_StateSinking from the next update.
 *
 * self: the lift.
 * Returns self->flags & 8. */
#include "xmen2.h"

short SentinelElevator_StateGoDown(register SentinelElevator *self)
{
	self->flags6 &= ~0x4000;
	self->vy = 0x100;
	SET_STATE(self, SentinelElevator_StateSinking);
	return self->flags & 8;
}
