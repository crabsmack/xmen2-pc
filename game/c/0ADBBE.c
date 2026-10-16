/* TrapDoor_StateArm - TRAPDOOR state: armed (flags6 bit $1000 off), wait for a player from the
 * next update (TrapDoor_StateWaitPlayer).
 *
 * self: the object. Returns self->flags & 8. */
#include "xmen2.h"

short TrapDoor_StateArm(TrapDoor *self)
{
	self->flags6 &= ~0x1000;
	SET_STATE(self, TrapDoor_StateWaitPlayer);
	return self->flags & 8;
}
