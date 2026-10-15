/* MagnetoBoss_FinalSetup - MAGNETOBOSS final fight: drift towards its point (moveMode 4; hits
 * push it) and shoot (MagnetoBoss_StateFinalFight, also the state to resume after a shot).
 *
 * self: the boss.
 * Returns self->flags & 8. */
#include "xmen2.h"

short MagnetoBoss_FinalSetup(register MagnetoBoss *self)
{
	self->resumeState = MagnetoBoss_StateFinalFight;
	self->moveMode = 4;
	SET_STATE(self, MagnetoBoss_StateFinalFight);
	return self->flags & 8;
}
