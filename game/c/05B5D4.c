/* Boulder_StateStart - first state of a carried BOULDER (Boulder_Spawn kind 1, placement param 1):
 * frame 0, not moving, then wait to be let go (Boulder_StateWaitGround).
 *
 * self: the boulder. Returns self->flags & 8. */
#include "xmen2.h"

short Boulder_StateStart(register Boulder *self)
{
	(*self->cls->method[20])(self, 0);		/* +$56 SetFrame */
	STOP_MOVING(self);
	SET_STATE(self, Boulder_StateWaitGround);
	return self->flags & 8;
}
