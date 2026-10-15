/* BroodBoss_StateCharge - BROODBOSS state: charge until the boss offset x reaches $1C4, then hit
 * the wall without breaking it: frame $D, the wall's hit animation (frame 1), screen shake
 * (Tbl_BroodBossQuake), the claw swings back (speed $A00, mode 0), the little leg walks back (2);
 * then BroodBoss_StateRetreat.
 *
 * self: the boss. Returns flags & 8. */
#include "xmen2.h"

short BroodBoss_StateCharge(BroodBoss *self)
{
	if ((g_BroodBossOffsetX >> 8) < 0x1C4) {
		SET_STATE(self, BroodBoss_StateCharge);
		return self->flags & 8;
	}
	(*self->cls->method[20])(self, 0xD);		/* +$56 SetFrame */
	if (self->wall) {
		ICandy_SetAnim(self->wall, CODEPTR(Tbl_BroodBossWallHitAnim));
		self->wall->frame = 1;
	}
	g_CamShake = CODEPTR(Tbl_BroodBossQuake);
	self->claw->vx = 0xA00;
	self->claw->mode = 0;
	self->claw->motion = BroodClaw_Swing;
	if (self->leg)
		LittleLeg_SetWalking(self->leg, 2);
	SET_STATE(self, BroodBoss_StateRetreat);
	return self->flags & 8;
}
