/* Barbarian_StateTryLeap2 - BARBARIAN state: try the leap again after walking (flags6 bit $4000
 * on): leap when in range (Barbarian_SetupLeap, Barbarian_StateLeap), else after 30 frames aim at
 * the near edge of the target segment (Barbarian_StateToLeapEdge).
 *
 * self: the object. Never runs in the corpus.
 * Returns self->flags & 8. */
#include "xmen2.h"

short Barbarian_StateTryLeap2(Barbarian *self)
{
	self->flags6 |= 0x4000;
	if (Barbarian_SetupLeap(self)) {
		SET_STATE(self, Barbarian_StateLeap);
		return self->flags & 8;
	}
	SET_STATE_WAIT(self, Barbarian_StateToLeapEdge, 30);
	return self->flags & 8;
}
