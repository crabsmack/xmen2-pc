/* MagnetoBoss_Zone2Setup - MAGNETOBOSS chase, part 2: hover beside the target (moveMode 2, $73
 * px right of it, $5A above), a shot every 90 frames (MagnetoBoss_StateZone2Approach, also the
 * state to resume after a shot).
 *
 * self: the boss.
 * Returns self->flags & 8. */
#include "xmen2.h"

short MagnetoBoss_Zone2Setup(register MagnetoBoss *self)
{
	self->resumeState = MagnetoBoss_StateZone2Approach;
	self->moveMode = 2;
	self->sideX = 0x73;
	self->sideY = -0x5A;
	self->shotPeriod = 90;
	SET_STATE(self, MagnetoBoss_StateZone2Approach);
	return self->flags & 8;
}
