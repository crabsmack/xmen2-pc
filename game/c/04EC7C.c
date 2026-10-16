/* BeamScooter_Init - BEAMSCOOTER constructor (class slot +$12): the parent's (ACTOR) Init, then no
 * motion, Art_ApocalypseSpr2's sprite (palette 3, priority bit set), flags6 |= $1283, draw layer
 * 2, first state BeamScooter_StateEnter.
 *
 * self: the new object.  x, y: position (8 fraction bits).  pl: its placement entry (passed on). */
#include "xmen2.h"

void BeamScooter_Init(register BeamScooter *self, long x, long y, Placement *pl)
{
	(*CLASS(Tbl_BeamScooter_vtable)->parent->method[3])(self, x, y, pl);	/* ACTOR Init */
	STOP_MOVING(self);
	self->spr.def = CODEPTR(Art_ApocalypseSpr2);
	self->spr.flags = self->spr.flags & ~3 | 3;
	self->flags6 |= 0x1283;
	self->spr.flags |= 4;
	self->layer = 2;
	self->state = BeamScooter_StateEnter;
}
