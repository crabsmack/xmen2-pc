/* RetractFloor_Init - RETRACTFLOOR constructor (class slot +$12): the parent's (ACTOR) Init, then
 * a closed floor piece waiting to be started.
 *
 * self: the new object.  x, y: position (8 fraction bits).  pl: its placement entry (only passed
 * on).
 * No vertical speed or gravity; flags6 |= $4083; sprite set g_RetractFloorSprDef, palette 0;
 * not started, halves closed; its floor segment is the type-$13 one in its 32-px map block
 * (Seg_FindInBlock); state (and default state) RetractFloor_StateInit. */
#include "xmen2.h"

void RetractFloor_Init(register RetractFloor *self, long x, long y, Placement *pl)
{
	register short col;
	register short row;
	register Seg *seg;

	(*CLASS(Tbl_RetractFloor_vtable)->parent->method[3])(self, x, y, pl);	/* ACTOR Init */
	self->vy = 0;
	self->ay = 0;
	self->flags6 |= 0x4083;
	Obj_SetSpriteDef((Actor *)self, g_RetractFloorSprDef);
	self->spr.flags = self->spr.flags & ~3 | 0;
	self->triggered = 0;
	self->offset = 0;
	col = self->x >> 13;
	row = self->y >> 13;
	seg = Seg_FindInBlock(0x13, col, row);
	self->floorSeg = seg;
	self->state = self->defaultState = RetractFloor_StateInit;
}
