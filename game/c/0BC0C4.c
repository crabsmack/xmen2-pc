/* SavageBio_StateDash - SAVAGEBIGBIO state: dash until within 15 px of the braking point
 * (+$A8), then brake against the motion (acceleration $18, SavageBio_StateDashBrake); at an
 * end of the arena stop and go on with the script.
 *
 * self: the boss (after SavageBio_ActDashPast/ActDashTo).
 * Returns self->flags & 8 unless the script goes on. */
#include "xmen2.h"

short SavageBio_StateDash(SavageBigBio *self)
{
	short x = self->x >> 8;

	if (ABS(x - self->dashBrakeX) < 0xF) {
		self->ax = -((self->spr.flags & 0x10) ? -0x18 : 0x18);
		SET_STATE(self, SavageBio_StateDashBrake);
		return self->flags & 8;
	} else if (x == self->arenaLeft || x == self->arenaRight) {
		STOP_MOVING(self);
		AI_NEXT_STEP(self, self->ai, 0);
	} else {
		SET_STATE(self, SavageBio_StateDash);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
