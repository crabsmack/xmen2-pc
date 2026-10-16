/* SentinelDoor_StateClose2 - SENTINELDOOR closing: frame 4; SentinelDoor_StateClose3 in 1 frame.
 *
 * self: the door.
 * Returns self->flags & 8. */
#include "xmen2.h"

short SentinelDoor_StateClose2(register SentinelDoor *self)
{
	(*self->cls->method[20])(self, 4);	/* +$56 SetFrame */
	SET_STATE_WAIT(self, SentinelDoor_StateClose3, 1);
	return self->flags & 8;
}
