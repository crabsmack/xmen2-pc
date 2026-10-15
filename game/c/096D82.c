/* XClone_ActChildShoot3 - XCLONE script action: make the balls form fire 3 shots at the nearest
 * player (XClone_StateChildShoot); with no BIOBALLS object go on with the script now.
 *
 * self: the clone (a step of its AI script; never runs in the corpus).
 * Returns flags & 8 when shooting; nothing in particular otherwise. */
#include "xmen2.h"

short XClone_ActChildShoot3(XClone *self)
{
	if (self->child) {
		self->shotsLeft = 3;
		SET_STATE(self, XClone_StateChildShoot);
		return self->flags & 8;
	}
	AI_NEXT_STEP(self, self->ai, 0);
	NO_RETURN_VALUE
}
