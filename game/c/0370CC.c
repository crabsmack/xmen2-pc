/* MagnetoBoss_Zone1Exit - MAGNETOBOSS leaves camera zone 1: hover beside the target again
 * (moveMode 2) and fly on to smash the floor (MagnetoBoss_StateFlyToSmash1, also the state to
 * resume after a shot).
 *
 * self: the boss.
 * Returns self->flags & 8. */
#include "xmen2.h"

short MagnetoBoss_Zone1Exit(register MagnetoBoss *self)
{
	self->resumeState = MagnetoBoss_StateFlyToSmash1;
	self->moveMode = 2;
	SET_STATE(self, MagnetoBoss_StateFlyToSmash1);
	return self->flags & 8;
}
