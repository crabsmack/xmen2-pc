/* PerimeterDoor_StateWaitBroken - PERIMETERDOOR state: wait for the door's 11th hit
 * (g_PerimeterDoorHits, PerimeterDoor_TileHit), then crumble (PerimeterDoor_StateCrumble) from
 * the next update.
 *
 * self: the door.
 * Returns self->flags & 8 when it goes on; nothing in particular while waiting. */
#include "xmen2.h"

short PerimeterDoor_StateWaitBroken(PerimeterDoor *self)
{
	if (g_PerimeterDoorHits >= 11) {
		SET_STATE(self, PerimeterDoor_StateCrumble);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
