/* Tuskette_StateExitTurn - TUSKETTE leaving TUSK's arena, on the floor below: turn round and walk
 * at $200 back under its spawn point (Tuskette_StateExitBack).
 *
 * self: the object.
 * Returns self->flags & 8. */
#include "xmen2.h"

short Tuskette_StateExitTurn(Tuskette *self)
{
	self->spr.flags ^= 0x10;
	self->vx = self->spr.flags & 0x10 ? -0x200 : 0x200;
	SET_STATE(self, Tuskette_StateExitBack);
	return self->flags & 8;
}
