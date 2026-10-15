/* BroodBoss_StateChargeWall - BROODBOSS state: charge until the boss offset x reaches $1C4, then
 * stop the claw and the little leg and shake the screen (Tbl_BroodBossQuake); then
 * BroodBoss_StateHitWall.
 *
 * self: the boss. Returns flags & 8. */
#include "xmen2.h"

short BroodBoss_StateChargeWall(BroodBoss *self)
{
	if ((g_BroodBossOffsetX >> 8) < 0x1C4) {
		SET_STATE(self, BroodBoss_StateChargeWall);
		return self->flags & 8;
	}
	self->claw->vx = 0;
	self->claw->mode = 0;
	self->claw->motion = 0;
	if (self->leg)
		LittleLeg_SetWalking(self->leg, 0);
	g_CamShake = CODEPTR(Tbl_BroodBossQuake);
	SET_STATE(self, BroodBoss_StateHitWall);
	return self->flags & 8;
}
