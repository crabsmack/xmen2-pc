/* BroodBoss_StateSetup - BROODBOSS first state: no frame yet; create its parts: the claw
 * (BroodClaw_Spawn from a placement at x + $11, y $660; still, idle: BroodClaw_StateIdle), the
 * arm (BROODBOSSARM at x + $31, y - $C) and the little leg (LITTLELEG at x + $6A, y - $24),
 * x/y being its pixel position; its sprite again (Art_BroodBossSpr, palette 2); then
 * BroodBoss_StateRoar.
 *
 * self: the boss. Returns flags & 8. */
#include "xmen2.h"

short BroodBoss_StateSetup(BroodBoss *self)
{
	register short legX;
	register short y;
	register short x;
	register short legY;
	Placement pl;

	(*self->cls->method[20])(self, 0xFF);		/* +$56 SetFrame */
	x = self->px + -0x20;
	y = self->py + 0x5B;
	Mem_Set(&pl, 0, sizeof(pl));
	pl.x = x + 0x31;
	pl.y = 0x660;
	if (self->claw = BroodClaw_Spawn(&pl, 1)) {
		self->claw->vx = 0;
		self->claw->mode = 0;
		self->claw->motion = 0;
		self->claw->state = (StateFn)BroodClaw_StateIdle;
	}
	Mem_Set(&pl, 0, sizeof(pl));
	pl.x = self->px + 0x31;
	pl.y = self->py + -0xC;
	self->arm = (BroodBossArm *)(*CLASS(Tbl_BroodBossArm_vtable)->method[2])(0,	/* New */
		CLASS(Tbl_BroodBossArm_vtable), pl.x << 8, pl.y << 8, &pl);
	self->spr.def = CODEPTR(Art_BroodBossSpr);
	self->spr.flags = self->spr.flags & ~3 | 2;
	legX = x + 0x8A;
	legY = y + -0x7F;
	self->leg = (LittleLeg *)(*CLASS(Tbl_LittleLeg_vtable)->method[1])(0, legX, legY);	/* New */
	SET_STATE(self, BroodBoss_StateRoar);
	return self->flags & 8;
}
