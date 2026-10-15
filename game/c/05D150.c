/* Tuskette_StateWaitNear - TUSKETTE walker: wait until the nearest player is within `range`
 * pixels in x and 64 in y, then start walking (Tuskette_StateWalkStart).
 *
 * self: the object. Never runs in the corpus.
 * Returns self->flags & 8 then, nothing while waiting. */
#include "xmen2.h"

short Tuskette_StateWaitNear(Tuskette *self)
{
	XPlayer *p;

	p = Player_Nearest((Actor *)self);
	if (ABS(p->x - self->x) < self->range << 8 && ABS(p->y - self->y) < 0x4000) {
		SET_STATE(self, Tuskette_StateWalkStart);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
