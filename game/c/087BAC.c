/* BroodBoss_StateDeathStart - BROODBOSS state: start dying: the death animation
 * (Tbl_BroodBossAnim7), the claw swings out slowly (speed -$100, mode 0); then
 * BroodBoss_StateDeathSlide.
 *
 * self: the boss. Returns flags & 8. */
#include "xmen2.h"

short BroodBoss_StateDeathStart(BroodBoss *self)
{
	Obj_SetAnim(self, CODEPTR(Tbl_BroodBossAnim7));
	self->claw->vx = -0x100;
	self->claw->mode = 0;
	self->claw->motion = BroodClaw_Swing;
	SET_STATE(self, BroodBoss_StateDeathSlide);
	return self->flags & 8;
}
