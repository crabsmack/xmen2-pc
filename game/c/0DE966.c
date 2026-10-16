/* SentinelDoor_StateClose5 - SENTINELDOOR closing, last step: frame 1, no longer moving;
 * SentinelDoor_StateClosed in 1 frame.
 *
 * self: the door.
 * Returns self->flags & 8. */
#include "xmen2.h"

short SentinelDoor_StateClose5(register SentinelDoor *self)
{
	(*self->cls->method[20])(self, 1);	/* +$56 SetFrame */
	self->moving = 0;
	SET_STATE_WAIT(self, SentinelDoor_StateClosed, 1);
	return self->flags & 8;
}
