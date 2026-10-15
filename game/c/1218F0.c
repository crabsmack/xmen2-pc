/* BigBio_Init - BIGBIO constructor (class slot +$12): the parent's Init, then the boss set up to
 * wait for the players (BigBio_StateStart).
 *
 * self:  the new object.
 * x, y:  position (8 fraction bits).
 * place: its placement entry: x is the arena's left edge, a nonzero param gives the sprite high
 *        priority (not null-checked).
 * No motion, 50 health, orb mode 0, flags6 |= $80, sprite Art_BasicBioSentinelSpr with palette
 * 3, facing right, no orb shape. */
#include "xmen2.h"

void BigBio_Init(register BigBio *self, long x, long y, register Placement *place)
{
	(*CLASS(Tbl_BigBio_vtable)->parent->method[3])(self, x, y, place);	/* BASICBIOSENTINEL Init */
	STOP_MOVING(self);
	self->health = 50;
	self->orbMode = 0;
	self->state = BigBio_StateStart;
	self->defaultState = BigBio_StateStart;
	self->flags6 |= 0x80;
	self->spr.def = CODEPTR(Art_BasicBioSentinelSpr);
	self->spr.flags = self->spr.flags & ~3 | 3;
	if (place->param[0])
		self->spr.flags |= 4;
	else
		self->spr.flags &= ~4;
	self->arenaLeft = place->x;
	self->facingRight = 1;
	self->shape = 0;
}
