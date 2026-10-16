/* TrapDoor_StateOpenWide - TRAPDOOR state: wide open (its wide-open blocks, slot +$82,
 * TrapDoor_SetTiles), start closing in 10 frames (TrapDoor_StateClosing).
 *
 * self: the object. Returns self->flags & 8. */
#include "xmen2.h"

short TrapDoor_StateOpenWide(TrapDoor *self)
{
	(*self->cls->method[31])(self, self->tileWide);	/* +$82 TrapDoor_SetTiles */
	SET_STATE_WAIT(self, TrapDoor_StateClosing, 10);
	return self->flags & 8;
}
