/* MagnetoBoss_Zone0Setup - MAGNETOBOSS chase, part 0: hover beside the target (moveMode 2,
 * $82 px right of it, $6E above) and shoot (MagnetoBoss_StateZone0, also the state to resume
 * after a shot).
 *
 * self: the boss.
 * Returns self->flags & 8. */
#include "xmen2.h"

short MagnetoBoss_Zone0Setup(register MagnetoBoss *self)
{
	self->resumeState = MagnetoBoss_StateZone0;
	self->moveMode = 2;
	self->sideX = 0x82;
	self->sideY = -0x6E;
	SET_STATE(self, MagnetoBoss_StateZone0);
	return self->flags & 8;
}
