/* BioNpc_Init - BIONPC constructor (class slot +$12): the parent's (XNPC) Init, then not stunned
 * (stun frames 0, the saved palette bits kept) and flags6 |= $100.
 *
 * self: the new object.  x, y: position (8 fraction bits).  pl: its placement entry (or 0; only
 * passed on). */
#include "xmen2.h"

void BioNpc_Init(register BioNpc *self, long x, long y, Placement *pl)
{
	(*CLASS(Tbl_BioNpc_vtable)->parent->method[3])(self, x, y, pl);	/* XNPC Init */
	self->stun = self->stun & 0xC000 | 0;
	self->flags6 |= 0x100;
}
