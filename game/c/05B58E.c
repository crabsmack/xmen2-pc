/* Boulder_StateWaitGround - a carried BOULDER (TUSK's, spr.parent: the object carrying it) waits
 * to be let go; once free, it starts bouncing (Boulder_StateBounce) in 20 frames.
 *
 * self: the boulder. Returns self->flags & 8. */
#include "xmen2.h"

short Boulder_StateWaitGround(register Boulder *self)
{
	if (self->spr.parent != 0xFF) {
		SET_STATE(self, Boulder_StateWaitGround);
		return self->flags & 8;
	} else {
		SET_STATE_WAIT(self, Boulder_StateBounce, 20);
		return self->flags & 8;
	}
}
