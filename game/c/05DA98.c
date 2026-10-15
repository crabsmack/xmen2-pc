/* Tuskette_StateExitWalk - TUSKETTE leaving TUSK's arena: walk until more than 100 px (x) from
 * where it was spawned (px), then stop and drop (Tuskette_StateExitDrop).
 *
 * self: the object.
 * Returns self->flags & 8 then, nothing on the way. */
#include "xmen2.h"

short Tuskette_StateExitWalk(Tuskette *self)
{
	register long d;

	d = self->x - (self->px << 8);
	if (ABS(d) > 0x6400) {
		self->ax = 0;
		self->vx = 0;
		SET_STATE(self, Tuskette_StateExitDrop);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
