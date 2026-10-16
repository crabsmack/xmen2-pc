/* MaintRobotFire_Init - MAINTROBOTFIRE Init (class slot +$12): the parent's (ACTOR) Init, then no
 * gravity, sprite Art_MaintRobotFireSpr2 with palette 1, damage 1, flags6 |= $4000, 2, 8, $1000
 * (one at a time), draw layer $10 and its flame animation (Tbl_MaintRobotFireAnim).
 *
 * self: the flame.  x, y, pl: passed to the parent's Init. */
#include "xmen2.h"

void MaintRobotFire_Init(MaintRobotFire *self, long x, long y, Placement *pl)
{
	(*CLASS(Tbl_MaintRobotFire_vtable)->parent->method[3])(self, x, y, pl);	/* ACTOR Init */
	self->ay = 0;
	self->spr.def = CODEPTR(Art_MaintRobotFireSpr2);
	self->spr.flags = self->spr.flags & ~3 | 1;
	self->attack = 1;
	self->flags6 |= 0x4000;
	self->flags6 |= 2;
	self->flags6 |= 8;
	self->flags6 |= 0x1000;
	self->layer = 0x10;
	Obj_SetAnim(self, CODEPTR(Tbl_MaintRobotFireAnim));
}
