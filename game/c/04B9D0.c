/* Apocalypse_ActOrbitFrom0 - APOCALYPSE script action: circle from angle 0 with frame 0
 * (Apocalypse_StateOrbitStep from the next update).
 *
 * self: the boss. Returns self->flags & 8. */
#include "xmen2.h"

short Apocalypse_ActOrbitFrom0(register Apocalypse *self)
{
	self->angle = 0;
	(*self->cls->method[20])(self, 0);		/* +$56 SetFrame */
	SET_STATE(self, Apocalypse_StateOrbitStep);
	return self->flags & 8;
}
