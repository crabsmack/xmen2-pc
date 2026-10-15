/* BroodBoss_StateRemoveParts - BROODBOSS state: wait until the boss offset x is $68 or less, then
 * stop and kill the claw, kill the arm and destroy the little leg; then BroodBoss_StateFinish in
 * 60 frames.
 *
 * self: the boss. Returns flags & 8. */
#include "xmen2.h"

short BroodBoss_StateRemoveParts(BroodBoss *self)
{
	if ((g_BroodBossOffsetX >> 8) > 0x68) {
		SET_STATE(self, BroodBoss_StateRemoveParts);
		return self->flags & 8;
	}
	self->claw->vx = 0;
	self->claw->mode = 0;
	self->claw->motion = 0;
	(*self->claw->cls->method[19])(self->claw);	/* +$52 Kill */
	self->claw = 0;
	if (self->arm) {
		(*self->arm->cls->method[19])(self->arm);	/* +$52 Kill */
		self->arm = 0;
	}
	if (self->leg) {
		(*self->leg->cls->method[4])(self->leg);	/* +$16 Destroy */
		self->leg = 0;
	}
	SET_STATE_WAIT(self, (StateFn)BroodBoss_StateFinish, 0x3C);
	return self->flags & 8;
}
