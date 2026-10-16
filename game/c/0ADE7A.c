/* TrapDoor_StateClosed - TRAPDOOR state: closed again: its closed blocks (slot +$82,
 * TrapDoor_SetTiles), the segment holds again (flags bit 6 off), re-arm from the next update
 * (TrapDoor_StateArm).
 *
 * self: the object. Returns self->flags & 8. */
#include "xmen2.h"

short TrapDoor_StateClosed(TrapDoor *self)
{
	(*self->cls->method[31])(self, self->tileClosed);	/* +$82 TrapDoor_SetTiles */
	self->door->flags &= ~0x40;
	SET_STATE(self, TrapDoor_StateArm);
	return self->flags & 8;
}
