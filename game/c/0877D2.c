/* BroodBoss_StateAdvance - BROODBOSS state: advance until the boss offset x is $130 or less, then
 * stop the claw; then BroodBoss_StateMakeUnhittable.
 *
 * self: the boss. Returns flags & 8. */
#include "xmen2.h"

short BroodBoss_StateAdvance(BroodBoss *self)
{
	if ((g_BroodBossOffsetX >> 8) > 0x130) {
		SET_STATE(self, BroodBoss_StateAdvance);
		return self->flags & 8;
	}
	self->claw->vx = 0;
	self->claw->mode = 0;
	self->claw->motion = 0;
	SET_STATE(self, (StateFn)BroodBoss_StateMakeUnhittable);
	return self->flags & 8;
}
