/* SporeMine_Init - SPOREMINE Init (class slot +$12): the parent's (ACTOR) Init, then no
 * vertical speed or gravity, flags6 |= $4081, the spores' sprite set (g_SporeMineSprDef) with
 * palette 3, unread_8C and unread_90 0.
 *
 * self: the object.  x, y: position (8 fraction bits).  pl: its placement entry (passed on).
 * Never runs in the corpus: spores are records, not objects (SporeMine_SpawnFromRec). */
#include "xmen2.h"

void SporeMine_Init(SporeMine *self, long x, long y, Placement *pl)
{
	(*CLASS(Tbl_SporeMine_vtable)->parent->method[3])(self, x, y, pl);	/* ACTOR Init */
	self->vy = 0;
	self->ay = 0;
	self->flags6 |= 0x4081;
	Obj_SetSpriteDef((Actor *)self, g_SporeMineSprDef);
	self->spr.flags = self->spr.flags & ~3 | 3;
	self->unread_8C = 0;
	self->unread_90 = 0;
}
