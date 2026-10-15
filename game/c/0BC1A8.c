/* SavageBio_StateDashBrake - SAVAGEBIGBIO state: brake until within 15 px of the stopping point
 * (+$AA) or slower than 8, or at an end of the arena; then stop and go on with the script.
 *
 * self: the boss (after SavageBio_StateDash).
 * Returns self->flags & 8 while braking; nothing in particular when the script goes on. */
#include "xmen2.h"

short SavageBio_StateDashBrake(SavageBigBio *self)
{
	short x = self->x >> 8;

	if (x == self->arenaLeft || x == self->arenaRight) {
		STOP_MOVING(self);
		AI_NEXT_STEP(self, self->ai, 0);
	} else if (ABS(x - self->dashStopX) < 0xF || ABS(self->vx) < 8) {
		STOP_MOVING(self);
		AI_NEXT_STEP(self, self->ai, 0);
	} else {
		SET_STATE(self, SavageBio_StateDashBrake);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
