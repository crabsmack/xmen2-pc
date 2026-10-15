/* MagnetoBoss_Zone4Hover - MAGNETOBOSS in camera zone 4: hover beside the target mirrored
 * (moveMode 3) inside x $620-$700 (bossFlags bit 1, the limit-reached bits cleared), shooting
 * (MagnetoBoss_StateZone4, also the state to resume after a shot).
 *
 * self: the boss.
 * Returns self->flags & 8. */
#include "xmen2.h"

short MagnetoBoss_Zone4Hover(register MagnetoBoss *self)
{
	self->moveMode = 3;
	self->bossFlags &= ~0xC;
	self->bossFlags |= 2;
	self->minX = 0x620;
	self->maxX = 0x700;
	self->resumeState = MagnetoBoss_StateZone4;
	SET_STATE(self, MagnetoBoss_StateZone4);
	return self->flags & 8;
}
