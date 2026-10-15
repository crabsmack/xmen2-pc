/* Barbarian_StateLeapFailed - BARBARIAN state: the leap was out of range - leapMode 1 (at the
 * end of the walk, try again) and walk towards the target (Barbarian_StateWalkStart).
 *
 * self: the object. Never runs in the corpus.
 * Returns self->flags & 8. */
#include "xmen2.h"

short Barbarian_StateLeapFailed(Barbarian *self)
{
	self->leapMode = 1;
	SET_STATE(self, Barbarian_StateWalkStart);
	return self->flags & 8;
}
