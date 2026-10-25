/* PowerUp_Init - POWERUP constructor (class slot +$12): the parent's (ACTOR) Init, then a pick-up:
 * not moving, flags6 |= 3, sprite Art_PowerUpSpr (palette 1), sprite priority (bit 2) set when the
 * live player's (player 2's when player 1 is not alive) is, its lifetime the placement's param[0]
 * (none without a placement: 0, for ever), no frame shown (+$56 SetFrame $FF).
 *
 * self: the new object.  x, y: position (8 fraction bits).  pl: its placement entry, or 0. */
#include "xmen2.h"

void PowerUp_Init(register PowerUp *self, long x, long y, register Placement *pl)
{
	(*CLASS(Tbl_PowerUp_vtable)->parent->method[3])(self, x, y, pl);	/* ACTOR Init */
	STOP_MOVING(self);
	self->flags6 |= 3;
	self->spr.def = CODEPTR(Art_PowerUpSpr);
	self->spr.flags = self->spr.flags & ~3 | 1;
	if (PLAYER_LIVE->spr.flags & 4)
		self->spr.flags |= 4;
	self->lifetime = 0;
	if (pl)
		self->lifetime = pl->param[0];
	(*self->cls->method[20])(self, 0xFF);	/* +$56 SetFrame: none */
}
