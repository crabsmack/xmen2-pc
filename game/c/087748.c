/* BroodBoss_ActAdvance - BROODBOSS AI-script action: advance after the smash. If it is unhittable
 * (flags6 bit 1), it becomes hittable again (back in the enemy body hit list) with the animation
 * off on frame $10; the claw swings out (speed $200, mode 0), the little leg walks back (2);
 * then BroodBoss_StateAdvance.
 *
 * self: the boss. Returns flags & 8. */
#include "xmen2.h"

short BroodBoss_ActAdvance(BroodBoss *self)
{
	if (self->flags6 & 2) {
		self->flags6 &= ~2;
		self->bodyNode = HitList_Add(self, &g_HitListEnemyBody);
		self->anim = 0;
		(*self->cls->method[20])(self, 0x10);	/* +$56 SetFrame */
	}
	self->claw->vx = 0x200;
	self->claw->mode = 0;
	self->claw->motion = BroodClaw_Swing;
	if (self->leg)
		LittleLeg_SetWalking(self->leg, 2);
	SET_STATE(self, BroodBoss_StateAdvance);
	return self->flags & 8;
}
