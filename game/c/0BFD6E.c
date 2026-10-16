/* SwampPlatform_StateIdle - SWAMPPLATFORM state: back at its home height (py) on frame 0; runs
 * itself again after $FF frames (SwampPlatform_Move bobs it meanwhile).
 *
 * self: the platform. Returns self->flags & 8. */
#include "xmen2.h"

short SwampPlatform_StateIdle(register SwampPlatform *self)
{
	self->y = self->py << 8;
	(*self->cls->method[20])(self, 0);		/* +$56 SetFrame */
	SET_STATE_WAIT(self, SwampPlatform_StateIdle, 0xFF);
	return self->flags & 8;
}
