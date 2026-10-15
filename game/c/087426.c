/* BroodBoss_StateRetreat - BROODBOSS state: back off until the boss offset x is $180 or less, then
 * idle (Tbl_BroodBossAnim2), stop the claw and the little leg, next AI-script step in 40 frames.
 *
 * self: the boss. Returns flags & 8 while it backs off, nothing after. */
#include "xmen2.h"

short BroodBoss_StateRetreat(BroodBoss *self)
{
	if ((g_BroodBossOffsetX >> 8) > 0x180) {
		SET_STATE(self, BroodBoss_StateRetreat);
		return self->flags & 8;
	}
	Obj_SetAnim(self, CODEPTR(Tbl_BroodBossAnim2));
	self->claw->vx = 0;
	self->claw->mode = 0;
	self->claw->motion = 0;
	if (self->leg)
		LittleLeg_SetWalking(self->leg, 0);
	AI_NEXT_STEP(self, self->ai, 0x28);
	NO_RETURN_VALUE
}
