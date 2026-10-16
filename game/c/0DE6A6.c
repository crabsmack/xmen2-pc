/* SentinelDoor_StateStart - SENTINELDOOR's first state (SentinelDoor_Init):
 * SentinelDoor_StateClosed from the next update.
 *
 * self: the door.
 * Returns self->flags & 8. */
#include "xmen2.h"

short SentinelDoor_StateStart(register SentinelDoor *self)
{
	SET_STATE(self, SentinelDoor_StateClosed);
	return self->flags & 8;
}
