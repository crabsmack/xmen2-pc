/* SentinelElevator_StateStart - SENTINELELEVATOR's first state (SentinelElevator_Init): frame 0,
 * then wait for its switch (SentinelElevator_StateWaitSwitch from the next update).
 *
 * self: the lift.
 * Returns self->flags & 8. */
#include "xmen2.h"

short SentinelElevator_StateStart(register SentinelElevator *self)
{
	(*self->cls->method[20])(self, 0);	/* +$56 SetFrame */
	SET_STATE(self, SentinelElevator_StateWaitSwitch);
	return self->flags & 8;
}
