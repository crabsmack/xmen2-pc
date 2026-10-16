/* SentinelDoor_StateClose3 - SENTINELDOOR closing: frame 3; SentinelDoor_StateClose4 in 1 frame.
 *
 * self: the door.
 * Returns self->flags & 8. */
#include "xmen2.h"

short SentinelDoor_StateClose3(register SentinelDoor *self)
{
	(*self->cls->method[20])(self, 3);	/* +$56 SetFrame */
	SET_STATE_WAIT(self, SentinelDoor_StateClose4, 1);
	return self->flags & 8;
}
