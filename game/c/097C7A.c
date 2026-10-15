/* XClone_ActWaitStopped - XCLONE script action (and state): wait until it no longer moves
 * sideways, then stop, stand and go on with the script in 10 frames.
 *
 * self: the clone (a step of its AI script).
 * Returns flags & 8 while waiting; nothing in particular when the script goes on. */
#include "xmen2.h"

short XClone_ActWaitStopped(XClone *self)
{
	if (self->vx) {
		SET_STATE(self, XClone_ActWaitStopped);
		return self->flags & 8;
	}
	STOP_MOVING(self);
	Obj_ChangeAnim(self, CLONE_PART(self->record, standAnim));
	AI_NEXT_STEP(self, self->ai, 10);
	NO_RETURN_VALUE
}
