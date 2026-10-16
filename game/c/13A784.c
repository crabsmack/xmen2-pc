/* Mine_Init - MINE Init (class slot +$12): the parent's (ACTOR) Init, then no vertical speed or
 * gravity, flags6 |= $4081, the mines' sprite set (g_MineSprDef) with palette 3, unk_8C and
 * unk_90 0.
 *
 * self: the object.  x, y: position (8 fraction bits).  pl: its placement entry (passed on).
 * Never runs in the corpus: mines are records, not objects (Mine_SpawnFromRec). */
#include "xmen2.h"

void Mine_Init(Mine *self, long x, long y, Placement *pl)
{
	(*CLASS(Tbl_Mine_vtable)->parent->method[3])(self, x, y, pl);	/* ACTOR Init */
	self->vy = 0;
	self->ay = 0;
	self->flags6 |= 0x4081;
	Obj_SetSpriteDef((Actor *)self, g_MineSprDef);
	self->spr.flags = self->spr.flags & ~3 | 3;
	self->unk_8C = 0;
	self->unk_90 = 0;
}
