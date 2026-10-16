/* SentinelDoor_StateClose1 - SENTINELDOOR closing, first step: frame 5; SentinelDoor_StateClose2
 * in 1 frame.
 *
 * self: the door.
 * Returns self->flags & 8. */
#include "xmen2.h"

short SentinelDoor_StateClose1(register SentinelDoor *self)
{
	(*self->cls->method[20])(self, 5);	/* +$56 SetFrame */
	SET_STATE_WAIT(self, SentinelDoor_StateClose2, 1);
	return self->flags & 8;
}
