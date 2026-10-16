/* ClosedBarrier_Init - CLOSEDBARRIER constructor (class slot +$12): the parent's (ACTOR) Init, then
 * a BARRIER that is already shut (a BARRIER that closed despawns as one: Barrier_Despawn).
 *
 * self: the new object.  x, y: position (8 fraction bits).  pl: its placement entry.
 * Sprite set g_BarrierSprDef, palette 0; position = the placement's (pixels); flags6 |= $80, 1,
 * 2; sprite priority; not moving; the closed animation (Tbl_ClosedBarrierAnim) and its boxes
 * (Obj_UpdateBoxes). No state function. It has no floor block: the BARRIER's (Barrier_Close)
 * went with it. */
#include "xmen2.h"

void ClosedBarrier_Init(register ClosedBarrier *self, long x, long y, register Placement *pl)
{
	(*CLASS(Tbl_ClosedBarrier_vtable)->parent->method[3])(self, x, y, pl);	/* ACTOR Init */
	Obj_SetSpriteDef((Actor *)self, g_BarrierSprDef);
	self->spr.flags = self->spr.flags & ~3 | 0;
	self->px = pl->x;
	self->py = pl->y;
	self->x = pl->x << 8;
	self->y = pl->y << 8;
	self->flags6 |= 0x80;
	self->flags6 |= 1;
	self->flags6 |= 2;
	self->spr.flags |= 4;
	STOP_MOVING(self);
	Obj_SetAnim(self, CODEPTR(Tbl_ClosedBarrierAnim));
	Obj_UpdateBoxes(self);
}
