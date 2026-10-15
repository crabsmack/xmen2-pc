/* Exodus_StateStart - EXODUS's first state: wait 50 frames, then float down
 * (Exodus_StateDescend).
 *
 * self: the boss. Returns self->flags & 8. */
#include "xmen2.h"

short Exodus_StateStart(register Exodus *self)
{
	SET_STATE_WAIT(self, Exodus_StateDescend, 50);
	return self->flags & 8;
}
