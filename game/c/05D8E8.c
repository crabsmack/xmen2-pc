/* Tuskette_StateRunRightUntil - TUSKETTE runner running right: at its right turning point
 * (x >= xMax) stop for a moment (Tuskette_StateRunRightEnd).
 *
 * self: the object. The speed and acceleration aren't touched here or there.
 * Returns self->flags & 8 then, nothing while running. */
#include "xmen2.h"

short Tuskette_StateRunRightUntil(Tuskette *self)
{
	if (self->xMax << 8 <= self->x) {
		SET_STATE(self, Tuskette_StateRunRightEnd);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
