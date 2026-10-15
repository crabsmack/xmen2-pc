/* Tuskette_StateExitFall2 - TUSKETTE leaving TUSK's arena, dropping again: once more than 250 px
 * below the camera's top, remove it (Tuskette_StateRemove).
 *
 * self: the object.
 * Returns self->flags & 8 then, nothing on the way. */
#include "xmen2.h"

short Tuskette_StateExitFall2(Tuskette *self)
{
	register long d;

	d = self->y - (g_CameraY << 8);
	if (d > 0xFA00) {
		SET_STATE(self, Tuskette_StateRemove);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
