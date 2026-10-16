/* ExodusBlast_Init - EXODUSBLAST Init (class slot +$12): the parent's (XNPC) Init, then flags6
 * |= $7082, its sprite definition (g_ExodusBlastSprDef) with sprite flag bit 2 set and palette
 * 1, state and default state ExodusBlast_StateStart, two draw layers further back, no trail
 * step and no kind yet (ExodusBlast_Spawn sets it).
 *
 * self: the blast.  x, y: position (8 fraction bits).  pl: its placement entry (0 from Spawn).
 * It ends with a test of pl that does nothing (what it guarded was compiled out); the 68000
 * leaves pl in d0 (no caller reads it). */
#include "xmen2.h"

void ExodusBlast_Init(register ExodusBlast *self, long x, long y, register Placement *pl)
{
	(*CLASS(Tbl_ExodusBlast_vtable)->parent->method[3])(self, x, y, pl);	/* XNPC Init */
	self->flags6 |= 0x7082;
	if (1)	/* (a block: MPW pops the call arguments at its end, as the ROM does) */
		Obj_SetSpriteDef((Actor *)self, g_ExodusBlastSprDef);
	self->spr.flags |= 4;
	self->spr.flags = self->spr.flags & ~3 | 1;
	self->state = self->defaultState = ExodusBlast_StateStart;
	self->layer -= 2;
	self->trailDx = 0;
	self->trailDy = 0;
	self->kind = 0;
	self->unread_99 = 0;
	if (pl) {
	}
}
