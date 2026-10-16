/* SentinelDoor_StateOpen4 - SENTINELDOOR opening: frame 4; SentinelDoor_StateOpen5 in 1 frame.
 *
 * self: the door.
 * Returns self->flags & 8. */
#include "xmen2.h"

short SentinelDoor_StateOpen4(register SentinelDoor *self)
{
	(*self->cls->method[20])(self, 4);	/* +$56 SetFrame */
	SET_STATE_WAIT(self, SentinelDoor_StateOpen5, 1);
	return self->flags & 8;
}
