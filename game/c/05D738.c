/* Tuskette_StateWanderOut - TUSKETTE of a pair without its partner: keep moving until more than
 * 128 px (x) from where it was placed (px), then stop (Tuskette_StateStop).
 *
 * self: the object. Never runs in the corpus.
 * Returns self->flags & 8 then, nothing on the way. */
#include "xmen2.h"

short Tuskette_StateWanderOut(Tuskette *self)
{
	register long d;

	d = self->x - (self->px << 8);
	if (ABS(d) > 0x8000) {
		SET_STATE(self, Tuskette_StateStop);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
