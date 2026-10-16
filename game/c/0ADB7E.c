/* TrapDoor_StateStart - TRAPDOOR's first state: take it off the draw list (the map blocks
 * draw it), then arm it from the next update (TrapDoor_StateArm).
 *
 * self: the object. Returns self->flags & 8. */
#include "xmen2.h"

short TrapDoor_StateStart(TrapDoor *self)
{
	if (self->drawNode > -1) {
		DrawList_Remove(self->drawNode);
		self->drawNode = -1;
	}
	SET_STATE(self, TrapDoor_StateArm);
	return self->flags & 8;
}
