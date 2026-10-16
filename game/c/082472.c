/* DropBomb_StateWaitInView - DROPBOMB state: wait until the body box is on the screen (left at most
 * $110 px right of the camera, right at least $30 px right of it, top at most $E0 below, bottom
 * not above it), then (flags6 |= $1000) fall after dropDelay frames (DropBomb_StateDrop; at once
 * when the delay is -1).
 *
 * self: the bomb.
 * Returns flags & 8, or what DropBomb_StateDrop returns; nothing while still waiting (state
 * functions' results are not read there). */
#include "xmen2.h"

short DropBomb_StateWaitInView(DropBomb *self)
{
	if (self->boxLeft - g_CameraX <= 0x110 && self->boxRight - g_CameraX >= 0x30
	    && self->boxTop - g_CameraY <= 0xE0 && self->boxBottom - g_CameraY >= 0) {
		self->flags6 |= 0x1000;
		if (self->dropDelay == -1) {
			self->state = (StateFn)DropBomb_StateDrop;
			return DropBomb_StateDrop(self);
		}
		SET_STATE_WAIT(self, DropBomb_StateDrop, self->dropDelay);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
