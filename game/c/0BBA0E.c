/* SavageBio_StateChargeToEdge - SAVAGEBIGBIO state: charge until at either end of the arena,
 * then stop and go on with the script.
 *
 * self: the boss (after SavageBio_ActCharge; never runs in the corpus).
 * Returns self->flags & 8 while charging; nothing in particular when the script goes on. */
#include "xmen2.h"

short SavageBio_StateChargeToEdge(SavageBigBio *self)
{
	short x = self->x >> 8;

	if (x == self->arenaLeft || x == self->arenaRight) {
		STOP_MOVING(self);
		AI_NEXT_STEP(self, self->ai, 0);
	} else {
		SET_STATE(self, SavageBio_StateChargeToEdge);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
