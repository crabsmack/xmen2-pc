/* BroodClaw_StateMoveToDrop - BROODCLAW state after the scroll: move (speed $340) to x $6BE; within
 * 5 px stop and drop the clone (BroodClaw_StateDropClone) from the next update.
 *
 * self: the claw (swinging out of scroll mode: mode 0).
 * Returns self->flags & 8, as every state. */
#include "xmen2.h"

short BroodClaw_StateMoveToDrop(BroodClaw *self)
{
	register short x = self->x >> 8;
	register short target = 0x6BE;

	self->mode = 0;
	if (ABS(target - x) <= 5) {
		STOP_MOVING(self);
		SET_STATE(self, BroodClaw_StateDropClone);
		return self->flags & 8;
	}
	self->vx = x > target ? -0x340 : 0x340;
	SET_STATE(self, BroodClaw_StateMoveToDrop);
	return self->flags & 8;
}
