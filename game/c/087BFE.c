/* BroodBoss_StateDeathSlide - BROODBOSS state: slide until the boss offset x reaches $1B2, then stop
 * the claw and set off a chain of explosions on the body (a plain ACTOR at its position, sprite
 * priority, Tbl_BroodBossDeathChain); then BroodBoss_StateDeathAnim.
 *
 * self: the boss. Returns flags & 8. */
#include "xmen2.h"

short BroodBoss_StateDeathSlide(BroodBoss *self)
{
	Actor *fx;

	if ((g_BroodBossOffsetX >> 8) < 0x1B2) {
		SET_STATE(self, BroodBoss_StateDeathSlide);
		return self->flags & 8;
	}
	self->claw->vx = 0;
	self->claw->mode = 0;
	self->claw->motion = 0;
	fx = (Actor *)(*CLASS(Tbl_Actor_vtable)->method[2])(0,		/* New */
		CLASS(Tbl_Actor_vtable), self->x, self->y, 0);
	if (fx) {
		fx->spr.flags |= 4;
		ExplodeFx_ChainRepeat(fx, CODEPTR(Tbl_BroodBossDeathChain), 0, -0x10, -0x10, 0);
	}
	SET_STATE(self, BroodBoss_StateDeathAnim);
	return self->flags & 8;
}
