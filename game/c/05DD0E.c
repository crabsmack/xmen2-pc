/* Tuskette_StateRemove - TUSKETTE out of view below the camera: kill itself (class slot +$52),
 * and stay in this state (wait $FF) in case it is still updated.
 *
 * self: the object.
 * Returns self->flags & 8. */
#include "xmen2.h"

short Tuskette_StateRemove(Tuskette *self)
{
	(*self->cls->method[19])(self);		/* +$52 Kill */
	SET_STATE_WAIT(self, Tuskette_StateRemove, 0xFF);
	return self->flags & 8;
}
