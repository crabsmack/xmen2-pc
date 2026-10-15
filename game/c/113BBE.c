/* XNpc_Init - XNPC Init (class slot +$12): the parent's (ACTOR) Init, then the default stats
 * record (Tbl_XNpcDefaultStats) and its health, no shake, no saved hurt state.
 *
 * self: the object.  x, y: position (8 fraction bits).  pl: its placement entry or 0.
 * Subclass Inits replace the stats record and health with their own. */
#include "xmen2.h"

void XNpc_Init(XNpc *self, long x, long y, Placement *pl)
{
	(*CLASS(Tbl_XNpc_vtable)->parent->method[3])(self, x, y, pl);	/* ACTOR Init */
	self->health = (self->stats = (NpcStats *)CODEPTR(Tbl_XNpcDefaultStats))->health;
	self->shake = 0;
	self->hurtState = 0;
end:	;	/* (here MPW C pops all the arguments, as the ROM does) */
}
