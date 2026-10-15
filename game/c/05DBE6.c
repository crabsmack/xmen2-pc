/* Tuskette_StateExitBack - TUSKETTE leaving TUSK's arena, walking back on the floor below: once
 * under its spawn x (px) to within one frame's speed, keep going (Tuskette_StateExitPast).
 *
 * self: the object.
 * Returns self->flags & 8 then, nothing on the way. */
#include "xmen2.h"

short Tuskette_StateExitBack(Tuskette *self)
{
	register long d;

	d = self->x - (self->px << 8);
	if (ABS(d) < ABS(self->vx)) {
		SET_STATE(self, Tuskette_StateExitPast);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
