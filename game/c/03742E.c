/* MagnetoBoss_Zone4Setup - MAGNETOBOSS chase, part 4: hover beside the target (moveMode 2, $73
 * px right of it, $5A above), a shot every 80 frames (MagnetoBoss_StateZone4Approach, also the
 * state to resume after a shot).
 *
 * self: the boss.
 * Returns self->flags & 8. */
#include "xmen2.h"

short MagnetoBoss_Zone4Setup(register MagnetoBoss *self)
{
	self->moveMode = 2;
	self->sideX = 0x73;
	self->sideY = -0x5A;
	self->shotPeriod = 80;
	self->resumeState = MagnetoBoss_StateZone4Approach;
	SET_STATE(self, MagnetoBoss_StateZone4Approach);
	return self->flags & 8;
}
