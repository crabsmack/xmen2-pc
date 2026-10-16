/* TrapDoor_StateClosing - TRAPDOOR state: closing (its half-open blocks again, slot +$82,
 * TrapDoor_SetTiles), closed in 8 frames (TrapDoor_StateClosed).
 *
 * self: the object. Returns self->flags & 8. */
#include "xmen2.h"

short TrapDoor_StateClosing(TrapDoor *self)
{
	(*self->cls->method[31])(self, self->tileOpen);	/* +$82 TrapDoor_SetTiles */
	SET_STATE_WAIT(self, TrapDoor_StateClosed, 8);
	return self->flags & 8;
}
