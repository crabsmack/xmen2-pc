/* SentinelDoor_Init - SENTINELDOOR constructor (class slot +$12): the parent's (ACTOR) Init, then
 * a closed door with a solid segment over its doorway.
 *
 * self: the new object.  x, y: position (8 fraction bits).
 * pl:   its placement entry; 0: one made on the stack (all params 0).
 * No vertical speed or gravity, state (and default state) SentinelDoor_StateStart, flags6 |=
 * $408B, layer 0, sprite definition Art_SentinelDoorSpr (set directly), palette 0. The doorway
 * segment: type $13 on its layer, 16 px either side of its x, from $70 px above its y down to
 * it. Placement param 0 is its switch number (out of 0-14: 0); its flag word is
 * g_SentinelsD1Flags[switch]. Not moving, frame 0, boxes updated (Obj_UpdateBoxes). */
#include "xmen2.h"

void SentinelDoor_Init(register SentinelDoor *self, register long x, register long y,
	register Placement *pl)
{
	Placement def;
	register short sx;
	register short sy;
	register short n;
	register short v;

	(*CLASS(Tbl_SentinelDoor_vtable)->parent->method[3])(self, x, y, pl);	/* ACTOR Init */
	self->vy = 0;
	self->ay = 0;
	self->state = SentinelDoor_StateStart;
	self->defaultState = SentinelDoor_StateStart;
	self->flags6 |= 0x408B;
	self->layer = 0;
	self->spr.def = CODEPTR(Art_SentinelDoorSpr);
	self->spr.flags = self->spr.flags & ~3 | 0;
	sx = self->x >> 8;
	sy = self->y >> 8;
	self->doorSeg = Seg_Create(0x13, self->layer, sx - 16, sy - 0x70, sx + 16, sy);
	if (!pl) {
		pl = &def;
		pl->x = (short)(x >> 8);
		pl->y = (short)(y >> 8);
		pl->param[0] = 0;
		pl->param[1] = 0;
		pl->param[2] = 0;
		pl->param[3] = 0;
		pl->param[4] = 0;
	}
	n = pl->param[0];
	self->switchNum = v = n;
	if (v >= 15 || v < 0) {
		v = 0;
		self->switchNum = 0;
	}
	self->switchFlags = &g_SentinelsD1Flags[v];
	self->moving = 0;
	(*self->cls->method[20])(self, 0);	/* +$56 SetFrame */
	Obj_UpdateBoxes(self);
end:	;	/* (a label: MPW C pops the calls' arguments before it and before the if, as the ROM) */
}
