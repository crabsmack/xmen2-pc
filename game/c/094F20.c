/* EvilDrop_StateDrop - EVILDROP state: start falling (vy $100, gravity $80), then
 * EvilDrop_StateFall.
 *
 * self: the capsule. Returns flags & 8. Never runs in the corpus. */
#include "xmen2.h"

short EvilDrop_StateDrop(EvilDrop *self)
{
	self->vy = 0x100;
	self->ay = 0x80;
	SET_STATE(self, EvilDrop_StateFall);
	return self->flags & 8;
}
