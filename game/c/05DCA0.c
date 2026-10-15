/* Tuskette_StateExitDrop2 - TUSKETTE leaving TUSK's arena, past its spawn point: drop again
 * (vy $200, gravity 10) and fall out of view (Tuskette_StateExitFall2).
 *
 * self: the object.
 * Returns self->flags & 8. */
#include "xmen2.h"

short Tuskette_StateExitDrop2(Tuskette *self)
{
	self->vy = 0x200;
	self->ay = 10;
	SET_STATE(self, Tuskette_StateExitFall2);
	return self->flags & 8;
}
