/* Tuskette_StateRunLeftUntil - TUSKETTE runner running left: at its left turning point
 * (x <= xMin) stop for a moment (Tuskette_StateRunLeftEnd).
 *
 * self: the object. The speed and acceleration aren't touched here or there.
 * Returns self->flags & 8 then, nothing while running. */
#include "xmen2.h"

short Tuskette_StateRunLeftUntil(Tuskette *self)
{
	if (self->xMin << 8 >= self->x) {
		SET_STATE(self, Tuskette_StateRunLeftEnd);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
