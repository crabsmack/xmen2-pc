/* XClone_StateWaitChild2 - XCLONE state: wait until the balls form is done (BioBalls_IsDone), then
 * go on with the script now. Like XClone_StateWaitChild without its null check (a missing
 * object counts as done anyway).
 *
 * self: the clone (state set by XClone_StateBallsReturn).
 * Returns flags & 8 while waiting; nothing in particular when the script goes on. */
#include "xmen2.h"

short XClone_StateWaitChild2(XClone *self)
{
	BioBalls *balls = self->child;

	if (BioBalls_IsDone(balls)) {
		AI_NEXT_STEP(self, self->ai, 0);
	} else {
		SET_STATE(self, XClone_StateWaitChild2);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
