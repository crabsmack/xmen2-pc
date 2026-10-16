/* Apocalypse_StateRotateTo0 - APOCALYPSE state (also a script step): circle (moveFlags bit 0)
 * until the angle is 0, then stop circling and go on with the AI script on the next update.
 *
 * self: the boss.
 * Returns self->flags & 8 while circling, nothing when the script goes on. */
#include "xmen2.h"

short Apocalypse_StateRotateTo0(register Apocalypse *self)
{
	self->moveFlags |= 1;
	if (!self->angle) {
		self->moveFlags &= ~1;
		AI_NEXT_STEP(self, self->ai, 0);
	} else {
		SET_STATE(self, Apocalypse_StateRotateTo0);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
