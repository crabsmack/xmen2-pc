/* MagnetoBoss_Zone2Exit - MAGNETOBOSS leaves camera zone 2: hover beside the target again
 * (moveMode 2) and fly on to smash the floor (MagnetoBoss_StateFlyToSmash2, also the state to
 * resume after a shot).
 *
 * self: the boss.
 * Returns self->flags & 8. */
#include "xmen2.h"

short MagnetoBoss_Zone2Exit(register MagnetoBoss *self)
{
	self->resumeState = MagnetoBoss_StateFlyToSmash2;
	self->moveMode = 2;
	SET_STATE(self, MagnetoBoss_StateFlyToSmash2);
	return self->flags & 8;
}
