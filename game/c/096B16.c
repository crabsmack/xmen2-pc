/* XClone_ActWaitChildFlag - XCLONE script action (and state): wait while the BIOBALLS object is
 * busy (ballFlags $200), then go on with the script now.
 *
 * self: the clone (a step of its AI script; never runs in the corpus).
 * Returns flags & 8 while waiting; nothing in particular when the script goes on. */
#include "xmen2.h"

short XClone_ActWaitChildFlag(XClone *self)
{
	BioBalls *balls = self->child;

	if (balls && balls->ballFlags & 0x200) {
		SET_STATE(self, XClone_ActWaitChildFlag);
		return self->flags & 8;
	}
	AI_NEXT_STEP(self, self->ai, 0);
	NO_RETURN_VALUE
}
