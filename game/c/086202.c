/* BroodBossArm_Init - BROODBOSSARM Init (class slot +$12): the parent's (BROODBOSSPARTS) Init, then
 * the boss's arm: no motion, sprite Art_BroodBossArmSpr (palette 2, priority), flags6 |= $5083,
 * the state never runs (timer $FF), layer $14, no frame yet ($FF; BroodBossArm_Draw takes the
 * frame BroodClaw_Swing sets).
 *
 * self: the arm.  x, y: position (8 fraction bits).  pl: its placement entry. */
#include "xmen2.h"

void BroodBossArm_Init(BroodBossArm *self, long x, long y, Placement *pl)
{
	(*CLASS(Tbl_BroodBossArm_vtable)->parent->method[3])(self, x, y, pl);	/* BROODBOSSPARTS Init */
	STOP_MOVING(self);
	self->spr.def = CODEPTR(Art_BroodBossArmSpr);
	self->spr.flags = self->spr.flags & ~3 | 2;
	self->flags6 |= 0x5083;
	self->timer = 0xFF;
	self->layer = 0x14;
	self->spr.flags |= 4;
	(*self->cls->method[20])(self, 0xFF);		/* +$56 SetFrame */
}
