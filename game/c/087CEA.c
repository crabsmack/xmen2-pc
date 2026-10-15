/* BroodBoss_StateDeathFx - BROODBOSS state: while the boss offset x is below $8C, raise it by $A0
 * a frame ($140 in two-player step mode; it is already past $8C when this state starts, so this
 * never runs); then another chain of explosions on the body (as BroodBoss_StateDeathSlide) with
 * the music ($30) stopped and sound $73; then BroodBoss_StateDeathSink.
 *
 * self: the boss. Returns flags & 8. */
#include "xmen2.h"

short BroodBoss_StateDeathFx(BroodBoss *self)
{
	register short step = 0xA0;
	Actor *fx;

	if (g_TwoPlayerStep)
		step <<= 1;
	if ((g_BroodBossOffsetX >> 8) < 0x8C) {
		g_BroodBossOffsetX += step;
		SET_STATE(self, BroodBoss_StateDeathFx);
		return self->flags & 8;
	}
	fx = (Actor *)(*CLASS(Tbl_Actor_vtable)->method[2])(0,		/* New */
		CLASS(Tbl_Actor_vtable), self->x, self->y, 0);
	if (fx) {
		fx->spr.flags |= 4;
		ExplodeFx_ChainRepeat(fx, CODEPTR(Tbl_BroodBossDeathChain), 0, -0x10, -0x10, 0);
		Snd_Stop(0x30);
		Snd_Stop(0x73);
		Snd_Play(0x73);
	}
	SET_STATE(self, BroodBoss_StateDeathSink);
	return self->flags & 8;
}
