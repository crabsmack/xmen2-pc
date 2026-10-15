/* Tuskette_StateTargetGone - TUSKETTE of a pair whose partner has no health left: keep going
 * (Tuskette_StateWanderOut).
 *
 * self: the object. Never runs in the corpus.
 * Returns self->flags & 8. */
#include "xmen2.h"

short Tuskette_StateTargetGone(Tuskette *self)
{
	SET_STATE(self, Tuskette_StateWanderOut);
	return self->flags & 8;
}
