/* Apocalypse_ActSetMoveRates - APOCALYPSE script action (Tbl_ApocalypseChoiceScript2): slower
 * damped moves from now on (lunge shift 3, approach shift 4), then the next step now.
 *
 * self: the boss (never runs in the corpus). */
#include "xmen2.h"

void Apocalypse_ActSetMoveRates(register Apocalypse *self)
{
	self->lungeShift = 3;
	self->goShift = 4;
	AI_NEXT_STEP(self, self->ai, 0);
}
