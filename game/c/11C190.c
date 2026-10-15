/* BabyBio_Init - BABYBIO Init (class slot +$12): the parent's (BIONPC) Init, then the home
 * position, sprite, patrol range and volley from the placement entry. The subclass Inits pick
 * the AI script (by variant).
 *
 * self: the object.  x, y: position (8 fraction bits).
 * pl:   its placement entry; 0: one made on the stack from x, y with all params 0.
 * Placement params: +6 / +8 extent left / right of x (0: 48; negative: that side is an open
 * edge it falls off, wallLeft / wallRight 0), +$A shots per volley (0: 1), +$C wait of the
 * scripted waits (0: 120), +$E variant (its low byte). The y range is placement y (floor) to 64
 * px above. Not moving, speed $100, no flame, no AI script, state and default state
 * BabyBio_StateStart; sprite Art_BabyBioSpr, palette 1, its priority bit (2) from the live
 * player (player 1 when neither is). */
#include "xmen2.h"

void BabyBio_Init(BabyBio *self, long x, long y, Placement *pl)
{
	Placement tmp;
	register short v;		/* the placement param being read */

	if (!pl) {
		pl = &tmp;
		pl->x = (short)(x >> 8);
		pl->y = (short)(y >> 8);
		pl->param[0] = 0;
		pl->param[1] = 0;
		pl->param[2] = 0;
		pl->param[3] = 0;
		pl->param[4] = 0;
	}
	(*CLASS(Tbl_BabyBio_vtable)->parent->method[3])(self, x, y, pl);	/* BIONPC Init */
	STOP_MOVING(self);
	self->px = pl->x;
	self->py = pl->y;
	self->homeX = pl->x << 8;
	self->homeY = pl->y << 8;
	self->x = self->homeX;
	self->y = self->homeY;
	do {
		self->spr.def = CODEPTR(Art_BabyBioSpr);
		self->spr.flags = self->spr.flags & ~3 | 1;
	} while (0);
	if (IS_ALIVE(&g_Player1)) {
		if (g_Player1.spr.flags & 4)
			self->spr.flags |= 4;
		else
			self->spr.flags &= ~4;
	} else if (IS_ALIVE(&g_Player2)) {
		if (g_Player2.spr.flags & 4)
			self->spr.flags |= 4;
		else
			self->spr.flags &= ~4;
	} else {
		if (g_Player1.spr.flags & 4)
			self->spr.flags |= 4;
		else
			self->spr.flags &= ~4;
	}
	AiScript_Set(&self->ai, 0);
	self->state = self->defaultState = (StateFn)BabyBio_StateStart;
	self->flags6 |= 0x80;
	self->speed = 0x100;
	self->flame = 0;
	self->bob = (HoverStep *)CODEPTR(Tbl_BabyBioBobOffsets);
	if ((v = pl->param[0])) {
		if (v > 0)
			self->wallLeft = 1;
		else
			self->wallLeft = 0;
		self->xMin = pl->x - ABS(v);
	} else {
		self->wallLeft = 1;
		self->xMin = pl->x - 0x30;
	}
	if ((v = pl->param[1])) {
		if (v > 0)
			self->wallRight = 1;
		else
			self->wallRight = 0;
		self->xMax = pl->x + ABS(v);
	} else {
		self->wallRight = 1;
		self->xMax = pl->x + 0x30;
	}
	self->floorY = pl->y;
	self->ceilY = self->floorY - 0x40;
	if ((v = pl->param[2]))
		self->shots = (u8)v;
	else
		self->shots = 1;
	self->shotsLeft = self->shots;
	if ((v = pl->param[3]))
		self->delay = (u8)v;
	else
		self->delay = 120;
	self->variant = (u8)pl->param[4];
}
