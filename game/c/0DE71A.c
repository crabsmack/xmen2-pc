/* SentinelDoor_StateOpen - SENTINELDOOR state: open (its doorway segment off, flags bit 6;
 * frame 6); once its switch's flag bit 1 clears, close (SentinelDoor_StateClose1 from the next
 * update).
 *
 * self: the door.
 * Returns self->flags & 8 when it starts closing, nothing while it stays open. */
#include "xmen2.h"

short SentinelDoor_StateOpen(SentinelDoor *self)
{
	register s16 *flags = self->switchFlags;

	self->doorSeg->flags |= 0x40;
	(*self->cls->method[20])(self, 6);	/* +$56 SetFrame */
	if (!(*flags & 2)) {
		self->moving = 2;
		SET_STATE(self, SentinelDoor_StateClose1);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
