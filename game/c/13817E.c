/* LaserCannon_Init - LASERCANNON Init (class slot +$12): the parent's (XNPC) Init, then no y
 * motion, stats Tbl_LaserCannonStats (and their health), state (and default state)
 * LaserCannon_StateStart, flags6 |= $4081, sprite Art_LaserCannonSpr with palette 3; from the
 * placement (0: all params 0): facing right when param 0 is non-zero (else h-flipped, facing
 * left), reload delay (param 1, 0: 90), shots per burst (param 2, 0: 3), shot delay (param 3,
 * 0: 40), tracking (param 4 non-zero); no target, a full burst loaded.
 *
 * self: the object.  x, y: position (8 fraction bits).  pl: its placement entry, or 0. */
#include "xmen2.h"

void LaserCannon_Init(LaserCannon *self, long x, long y, Placement *pl)
{
	Placement tmp;
	register short v;		/* the placement param being read */

	(*CLASS(Tbl_LaserCannon_vtable)->parent->method[3])(self, x, y, pl);	/* XNPC Init */
	self->vy = 0;
	self->ay = 0;
	self->health = (self->stats = (NpcStats *)CODEPTR(Tbl_LaserCannonStats))->health;
	self->state = (StateFn)LaserCannon_StateStart;
	self->defaultState = (StateFn)LaserCannon_StateStart;
	self->flags6 |= 0x4081;
	self->spr.def = CODEPTR(Art_LaserCannonSpr);
	self->spr.flags = self->spr.flags & ~3 | 3;
	if (!pl) {
		pl = &tmp;
		pl->param[0] = 0;
		pl->param[1] = 0;
		pl->param[2] = 0;
		pl->param[3] = 0;
		pl->param[4] = 0;
	}
	if ((v = pl->param[0]))
		self->spr.flags &= ~0x10;
	else
		self->spr.flags |= 0x10;
	if ((v = pl->param[1]))
		self->reloadDelay = v;
	else
		self->reloadDelay = 90;
	if ((v = pl->param[2]))
		self->burst = v;
	else
		self->burst = 3;
	if ((v = pl->param[3]))
		self->shotDelay = v;
	else
		self->shotDelay = 40;
	self->tracks = pl->param[4] ? 1 : 0;
	self->target = 0;
	self->unk_98 = 0;
	self->shotsLeft = self->burst;
}
