/* Apocalypse_ActOrbitFrom180 - APOCALYPSE script action: circle from angle 180 with frame $82
 * (Apocalypse_StateOrbitStep from the next update).
 *
 * self: the boss. Returns self->flags & 8. */
#include "xmen2.h"

short Apocalypse_ActOrbitFrom180(register Apocalypse *self)
{
	self->angle = 180;
	(*self->cls->method[20])(self, 0x82);		/* +$56 SetFrame */
	SET_STATE(self, Apocalypse_StateOrbitStep);
	return self->flags & 8;
}
