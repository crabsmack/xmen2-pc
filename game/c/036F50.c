/* MagnetoBoss_Zone1Setup - MAGNETOBOSS in camera zone 1: hover beside the target mirrored
 * (moveMode 3) inside x $1C4-$2A4 (bossFlags bit 1, the limit-reached bits cleared), shooting
 * (MagnetoBoss_StateZone1, also the state to resume after a shot).
 *
 * self: the boss.
 * Returns self->flags & 8. */
#include "xmen2.h"

short MagnetoBoss_Zone1Setup(register MagnetoBoss *self)
{
	self->moveMode = 3;
	self->bossFlags &= ~0xC;
	self->bossFlags |= 2;
	self->minX = 0x1C4;
	self->maxX = 0x2A4;
	self->resumeState = MagnetoBoss_StateZone1;
	SET_STATE(self, MagnetoBoss_StateZone1);
	return self->flags & 8;
}
