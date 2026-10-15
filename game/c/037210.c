/* MagnetoBoss_Zone2Hover - MAGNETOBOSS in camera zone 2: hover beside the target mirrored
 * (moveMode 3) inside x $3BF-$49F (bossFlags bit 1, the limit-reached bits cleared), shooting
 * (MagnetoBoss_StateZone2, also the state to resume after a shot).
 *
 * self: the boss.
 * Returns self->flags & 8. */
#include "xmen2.h"

short MagnetoBoss_Zone2Hover(register MagnetoBoss *self)
{
	self->resumeState = MagnetoBoss_StateZone2;
	self->moveMode = 3;
	self->bossFlags &= ~0xC;
	self->bossFlags |= 2;
	self->minX = 0x3BF;
	self->maxX = 0x49F;
	SET_STATE(self, MagnetoBoss_StateZone2);
	return self->flags & 8;
}
