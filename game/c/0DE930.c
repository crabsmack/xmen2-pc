/* SentinelDoor_StateClose4 - SENTINELDOOR closing: frame 2; SentinelDoor_StateClose5 in 1 frame.
 *
 * self: the door.
 * Returns self->flags & 8. */
#include "xmen2.h"

short SentinelDoor_StateClose4(register SentinelDoor *self)
{
	(*self->cls->method[20])(self, 2);	/* +$56 SetFrame */
	SET_STATE_WAIT(self, SentinelDoor_StateClose5, 1);
	return self->flags & 8;
}
