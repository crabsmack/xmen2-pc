/* ShootAcolyte_Init - SHOOTACOLYTE constructor (class slot +$12): the parent's (BIONPC) Init, then
 * an acolyte that walks and shoots inside an x range around its placement.
 *
 * self: the new object.  x, y: position (8 fraction bits).  pl: its placement entry, or null
 * (then every param counts as 0).
 * No vertical speed or gravity; AI script Tbl_ShootAcolyteScript (thunks
 * ShootAcolyte_Script: ActWalk, ActFacePlayer, ActShootBurst, ActWait30), started by
 * ShootAcolyte_ActRestartScript (the state and the default state); stats
 * Tbl_ShootAcolyteStats (health from them); flags6 |= $80; sprite Art_ShootAcolyteSpr,
 * palette 1. Placement params: +6 how far left of the placement x it walks (|v|, 0: $60), +8
 * how far right (|v|, 0: $60) (the sign of each is kept in leftPositive / rightPositive, which
 * nothing reads), +$A walkTime (0: 60), +$C burstShots (0: 2), +$E shotDelay (0: 60). */
#include "xmen2.h"

void ShootAcolyte_Init(register ShootAcolyte *self, long x, long y, register Placement *pl)
{
	Placement none;
	register short v;		/* the placement param being read */

	(*CLASS(Tbl_ShootAcolyte_vtable)->parent->method[3])(self, x, y, pl);	/* BIONPC Init */
	self->vy = 0;
	self->ay = 0;
	AiScript_Set(&self->ai, CODEPTR(Tbl_ShootAcolyteScript));
	self->state = (StateFn)ShootAcolyte_ActRestartScript;
	self->defaultState = (StateFn)ShootAcolyte_ActRestartScript;
	self->health = (self->stats = (NpcStats *)CODEPTR(Tbl_ShootAcolyteStats))->health;
	self->flags6 |= 0x80;
	self->spr.def = CODEPTR(Art_ShootAcolyteSpr);
	self->spr.flags = self->spr.flags & ~3 | 1;
	if (!pl) {
		pl = &none;
		pl->param[0] = 0;
		pl->param[1] = 0;
		pl->param[2] = 0;
		pl->param[3] = 0;
		pl->param[4] = 0;
	}
	if ((v = pl->param[0]) != 0) {
		if (v > 0)
			self->leftPositive = 1;
		else
			self->leftPositive = 0;
		self->rangeLeft = pl->x - ABS(v);
	} else {
		self->leftPositive = 1;
		self->rangeLeft = pl->x - 0x60;
	}
	if ((v = pl->param[1]) != 0) {
		if (v > 0)
			self->rightPositive = 1;
		else
			self->rightPositive = 0;
		self->rangeRight = pl->x + ABS(v);
	} else {
		self->rightPositive = 1;
		self->rangeRight = pl->x + 0x60;
	}
	if ((v = pl->param[2]) != 0)
		self->walkTime = v;
	else
		self->walkTime = 0x3C;
	if ((v = pl->param[3]) != 0)
		self->burstShots = v;
	else
		self->burstShots = 2;
	if ((v = pl->param[4]) != 0)
		self->shotDelay = v;
	else
		self->shotDelay = 0x3C;
	self->unread_9C = 0;
	self->unread_A0 = 0;
	self->shotsLeft = self->burstShots;
}
