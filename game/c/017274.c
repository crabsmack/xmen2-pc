/* FarAcolyte_Init - FARACOLYTE constructor (class slot +$12): the parent's (ACTOR) Init, then
 * an acolyte anchored at its placement that waits to come on screen.
 *
 * self: the new object.  x, y: position (8 fraction bits).  pl: its placement entry.
 * Anchor px/py and home = the placement position; no vertical speed or gravity; states
 * FarAcolyte_StateWaitOnScreen (also the default) and FarAcolyte_StateDie (hurt); flags6 |=
 * $4082; sprite Art_FarAcolyteSpr, palette 1; no orb or bolt. Placement params: +6 the half
 * width of the x range the player must enter (0: $60), +8 restDelay (0: 120). Hover table
 * Tbl_CapedAcolyteHoverOffsets. */
#include "xmen2.h"

void FarAcolyte_Init(register FarAcolyte *self, long x, long y, register Placement *pl)
{
	(*CLASS(Tbl_FarAcolyte_vtable)->parent->method[3])(self, x, y, pl);	/* ACTOR Init */
	self->px = pl->x;
	self->py = pl->y;
	self->homeX = (long)self->px << 8;
	self->homeY = (long)self->py << 8;
	self->vy = 0;
	self->ay = 0;
	self->state = FarAcolyte_StateWaitOnScreen;
	self->defaultState = FarAcolyte_StateWaitOnScreen;
	self->hurtState = (StateFn)FarAcolyte_StateDie;
	self->flags6 |= 0x4082;
	self->spr.def = CODEPTR(Art_FarAcolyteSpr);
	self->spr.flags = self->spr.flags & ~3 | 1;
	self->shotFlags = 0;
	if (pl->param[0]) {
		self->rangeLeft = pl->x - pl->param[0];
		self->rangeRight = pl->param[0] + pl->x;
	} else {
		self->rangeLeft = pl->x - 0x60;
		self->rangeRight = pl->x + 0x60;
	}
	if (pl->param[1])
		self->restDelay = pl->param[1];
	else
		self->restDelay = 0x78;
	{
		register FarAcolyteShot *s;

		s = &self->orb;
		s->x = 0;
		s->y = 0;
		s->vx = 0;
		s->vy = 0;
		s->frame = 0;
		s = &self->bolt;
		s->x = 0;
		s->y = 0;
		s->vx = 0;
		s->vy = 0;
		s->frame = 0;
		s->frameTimer = 0;
	}
	self->hover = (HoverStep *)CODEPTR(Tbl_CapedAcolyteHoverOffsets);
}
