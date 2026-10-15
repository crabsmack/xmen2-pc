/* Tuskette_StateWaitSameFloor - TUSKETTE of a pair (kinds 6/7), first state: wait until the
 * nearest player stands on its floor segment, then start 20 frames later
 * (Tuskette_StateChaseStart).
 *
 * self: the object. Never runs in the corpus.
 * Returns self->flags & 8 then, nothing while waiting. */
#include "xmen2.h"

short Tuskette_StateWaitSameFloor(Tuskette *self)
{
	XPlayer *p;

	p = Player_Nearest((Actor *)self);
	if (p->seg == self->seg) {
		SET_STATE_WAIT(self, Tuskette_StateChaseStart, 20);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
