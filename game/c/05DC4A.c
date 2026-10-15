/* Tuskette_StateExitPast - TUSKETTE leaving TUSK's arena: walk on until more than 200 px (x) past
 * its spawn x (px), then stop and drop again (Tuskette_StateExitDrop2).
 *
 * self: the object.
 * Returns self->flags & 8 then, nothing on the way. */
#include "xmen2.h"

short Tuskette_StateExitPast(Tuskette *self)
{
	register long d;

	d = self->x - (self->px << 8);
	if (ABS(d) > 0xC800) {
		self->ax = 0;
		self->vx = 0;
		SET_STATE(self, Tuskette_StateExitDrop2);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
