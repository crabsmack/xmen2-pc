/* XClone_StateWaitChild - XCLONE state: wait until its BIOBALLS object is done
 * (BioBalls_IsDone), then go on with the script now.
 *
 * self: the clone (state set by the actions that start a balls effect).
 * With no BIOBALLS object it goes on at once.
 * Returns flags & 8 while waiting; nothing in particular when the script goes on. */
#include "xmen2.h"

short XClone_StateWaitChild(XClone *self)
{
	if (self->child && !BioBalls_IsDone(self->child)) {
		SET_STATE(self, XClone_StateWaitChild);
		return self->flags & 8;
	}
	AI_NEXT_STEP(self, self->ai, 0);
	NO_RETURN_VALUE
}
