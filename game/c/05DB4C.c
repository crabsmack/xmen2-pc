/* Tuskette_StateExitFall - TUSKETTE leaving TUSK's arena, dropping: once more than 119 px below
 * where it was spawned (py), stop falling and turn round (Tuskette_StateExitTurn).
 *
 * self: the object.
 * Returns self->flags & 8 then, nothing on the way. */
#include "xmen2.h"

short Tuskette_StateExitFall(Tuskette *self)
{
	register long d;

	d = self->y - (self->py << 8);
	if (ABS(d) > 0x7700) {
		self->vy = 0;
		self->ay = 0;
		SET_STATE(self, Tuskette_StateExitTurn);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
