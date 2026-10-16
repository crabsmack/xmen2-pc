/* Blast_Attack - BLAST Attack (class slot +$22): the parent's (ACTOR), then latch onto what it hit:
 * stop, no animation, no frame shown ($FF), the target kept (target, blastFlags bit 0), and
 * Blast_Latch next frame.
 *
 * self: the blast.  target: the object hit.  type, damage, x, y: the hit (passed on). */
#include "xmen2.h"

void Blast_Attack(register Blast *self, register Actor *target, char type, char damage, short x, short y)
{
	(*CLASS(Tbl_Blast_vtable)->parent->method[7])(self, target, type, damage, x, y);	/* ACTOR Attack */
	STOP_MOVING(self);
	self->anim = 0;
	(*self->cls->method[20])(self, 0xFF);		/* +$56 SetFrame: none */
	self->target = target;
	self->blastFlags |= 1;
	self->state = Blast_Latch;
	self->timer = 0;
}
