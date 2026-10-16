/* SentinelDoor_StateOpen5 - SENTINELDOOR opening, last step: frame 5, no longer moving;
 * SentinelDoor_StateOpen in 1 frame.
 *
 * self: the door.
 * Returns self->flags & 8. */
#include "xmen2.h"

short SentinelDoor_StateOpen5(register SentinelDoor *self)
{
	(*self->cls->method[20])(self, 5);	/* +$56 SetFrame */
	self->moving = 0;
	SET_STATE_WAIT(self, SentinelDoor_StateOpen, 1);
	return self->flags & 8;
}
