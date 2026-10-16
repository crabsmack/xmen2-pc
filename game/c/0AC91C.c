/* PerimeterDoor_StateStart - PERIMETERDOOR first state: leave the draw list (the door is map tiles,
 * not a sprite) and wait for it to break (PerimeterDoor_StateWaitBroken) from the next update.
 *
 * self: the door.
 * Returns self->flags & 8, as every state. */
#include "xmen2.h"

short PerimeterDoor_StateStart(PerimeterDoor *self)
{
	if (self->drawNode > -1)
		DrawList_Remove(self->drawNode);
	SET_STATE(self, PerimeterDoor_StateWaitBroken);
	return self->flags & 8;
}
