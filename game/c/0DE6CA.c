/* SentinelDoor_StateClosed - SENTINELDOOR state: closed (its doorway segment solid, frame 0);
 * once its switch's flag bit 1 is set, open (SentinelDoor_StateOpen1 from the next update).
 *
 * self: the door.
 * Returns self->flags & 8 when it starts opening, nothing while it stays closed. */
#include "xmen2.h"

short SentinelDoor_StateClosed(SentinelDoor *self)
{
	register s16 *flags = self->switchFlags;

	self->doorSeg->flags &= ~0x40;
	(*self->cls->method[20])(self, 0);	/* +$56 SetFrame */
	if (*flags & 2) {
		self->moving = 1;
		SET_STATE(self, SentinelDoor_StateOpen1);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
