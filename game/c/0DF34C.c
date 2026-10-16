/* SentinelElevator_StateAuto - SENTINELELEVATOR's first state when automatic (placement param 4
 * = 99): frame 0, waits of $3C frames at the top and $78 at the bottom, and
 * SentinelElevator_StateGoUp in 12 frames.
 *
 * self: the lift.
 * Returns self->flags & 8. */
#include "xmen2.h"

short SentinelElevator_StateAuto(register SentinelElevator *self)
{
	(*self->cls->method[20])(self, 0);	/* +$56 SetFrame */
	self->topWait = 0x3C;
	self->bottomWait = 0x78;
	SET_STATE_WAIT(self, SentinelElevator_StateGoUp, 12);
	return self->flags & 8;
}
