/* DropBomb_Init - DROPBOMB Init (class slot +$12): the parent's (ACTOR) Init, then a waiting bomb:
 * no vertical speed or gravity, state and default state DropBomb_StateStart, flags6 |= $4083,
 * sprite Art_DropBombSpr (palette 3); the drop delay from placement param 0 (0: $78) and the
 * floor it explodes on: the higher end of the floor segment below its placement (searched from
 * 32 px down, layers up to 1000), or $A0 px below it.
 *
 * self: the bomb.  x, y: position (8 fraction bits).  pl: its placement entry, or null (then a
 * local one: the pixel position of x/y, delay $78, other params 0; DropBomb_Spawn passes it). */
#include "xmen2.h"

void DropBomb_Init(DropBomb *self, long x, long y, Placement *pl)
{
	Placement none;
	short v;
	short px;
	short py;
	Seg *seg;

	(*CLASS(Tbl_DropBomb_vtable)->parent->method[3])(self, x, y, pl);	/* ACTOR Init */
	self->vy = 0;
	self->ay = 0;
	self->state = (StateFn)DropBomb_StateStart;
	self->defaultState = (StateFn)DropBomb_StateStart;
	self->flags6 |= 0x4083;
	self->spr.def = CODEPTR(Art_DropBombSpr);
	self->spr.flags = self->spr.flags & ~3 | 3;
	if (!pl) {
		pl = &none;
		pl->x = (short)(x >> 8);
		pl->y = (short)(y >> 8);
		pl->param[0] = 0x78;
		pl->param[1] = 0;
		pl->param[2] = 0;
		pl->param[3] = 0;
		pl->param[4] = 0;
	}
	if ((v = pl->param[0]) != 0)
		self->dropDelay = v;
	else
		self->dropDelay = 0x78;
	px = pl->x;
	py = pl->y;
	if ((seg = Seg_FindBelow(px, py + 0x20, 1000)) != 0)
		self->floorY = MIN(seg->y1, seg->y2);
	else
		self->floorY = py + 0xA0;
}
