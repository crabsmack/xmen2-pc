/* BroodBoss_ActChargeWall - BROODBOSS AI-script action: charge at the wall: animation off on frame
 * 6, the claw swings in fast (speed -$A00, mode 0), the little leg walks (1); then
 * BroodBoss_StateChargeWall.
 *
 * self: the boss. Returns flags & 8. */
#include "xmen2.h"

short BroodBoss_ActChargeWall(BroodBoss *self)
{
	self->anim = 0;
	(*self->cls->method[20])(self, 6);		/* +$56 SetFrame */
	self->claw->vx = -0xA00;
	self->claw->mode = 0;
	self->claw->motion = BroodClaw_Swing;
	if (self->leg)
		LittleLeg_SetWalking(self->leg, 1);
	SET_STATE(self, BroodBoss_StateChargeWall);
	return self->flags & 8;
}
