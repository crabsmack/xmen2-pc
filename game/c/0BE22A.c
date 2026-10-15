/* Barbarian_StateLeapEdge - BARBARIAN state: aim at the near edge of the target segment (15 px
 * in from its left end when facing right, from its right end when facing left) and leap when in
 * range (Barbarian_SetupLeap, Barbarian_StateLeap); else give up after 30 frames
 * (Barbarian_StateGiveUp).
 *
 * self: the object. Never runs in the corpus.
 * Returns self->flags & 8. */
#include "xmen2.h"

short Barbarian_StateLeapEdge(Barbarian *self)
{
	if (!(self->spr.flags & 0x10))
		self->targetX = self->targetSeg->x1 + 15;
	else
		self->targetX = self->targetSeg->x2 - 15;
	if (Barbarian_SetupLeap(self)) {
		SET_STATE(self, Barbarian_StateLeap);
		return self->flags & 8;
	}
	SET_STATE_WAIT(self, Barbarian_StateGiveUp, 30);
	return self->flags & 8;
}
