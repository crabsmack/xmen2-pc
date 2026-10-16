/* Blast_Init - BLAST constructor (class slot +$12): the parent's (ACTOR) Init, then an energy blast
 * about to charge up (Blast_StateStart).
 *
 * self: the new object.  x, y: position (8 fraction bits).
 * pl: its placement entry; 0: one made on the stack from x, y (switch 0, speed $100, the rest 0).
 * No vertical speed or gravity; state (and default state) Blast_StateStart; flags6 |= $82;
 * sprite set g_BarrierSprDef, palette 0, priority; snapped to the nearest floor
 * (Obj_SnapToNearestFloor(self, 1000, -1); the segment is not used). From the placement: switch
 * number (param 0), speed = |param 1| (0: $100), negated when param 2 is set, sticks = the low
 * byte of param 4; the switch's flag word in g_SentinelsD1Flags; no zaps, not latched, no target. */
#include "xmen2.h"

void Blast_Init(Blast *self, long x, long y, Placement *pl)
{
	Placement tmp;
	register Seg *seg;
	register short v;		/* the placement param being read */

	(*CLASS(Tbl_Blast_vtable)->parent->method[3])(self, x, y, pl);	/* ACTOR Init */
	self->vy = 0;
	self->ay = 0;
	self->state = Blast_StateStart;
	self->defaultState = Blast_StateStart;
	self->flags6 |= 0x82;
	Obj_SetSpriteDef((Actor *)self, g_BarrierSprDef);
	self->spr.flags = self->spr.flags & ~3 | 0;
	self->spr.flags |= 4;
	seg = Obj_SnapToNearestFloor(self, 1000, -1);
	if (seg) {
	}
	if (!pl) {
		pl = &tmp;
		pl->x = x >> 8;
		pl->y = y >> 8;
		pl->param[0] = 0;
		pl->param[1] = 0x100;
		pl->param[2] = 0;
		pl->param[3] = 0;
		pl->param[4] = 0;
	}
	self->switchNum = pl->param[0];
	if (v = pl->param[1])
		self->speed = ABS(v);
	else
		self->speed = 0x100;
	if (v = pl->param[2])
		self->speed = -self->speed;
	self->sticks = pl->param[4];
	self->switchFlags = &g_SentinelsD1Flags[self->switchNum];
	self->zaps = 0;
	self->blastFlags = 0;
	self->target = 0;
}
