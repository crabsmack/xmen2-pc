/* Pop_Init - POP constructor (class slot +$12): the parent's (ACTOR) Init, then an invisible
 * object waiting to turn into an explosion effect (Pop_StateStart, also its default state).
 *
 * self: the new object.  x, y: position (8 fraction bits).  pl: its placement entry (never null).
 * flags6 |= $80, not moving, no frame shown ($FF). Placement params: 0 fxType (the effect,
 * Pop_StateWait), 1 the frames before it (countdown; 0: 60).
 * Never runs in the corpus. */
#include "xmen2.h"

void Pop_Init(register Pop *self, long x, long y, register Placement *pl)
{
	(*CLASS(Tbl_Pop_vtable)->parent->method[3])(self, x, y, pl);	/* ACTOR Init */
	self->state = self->defaultState = Pop_StateStart;
	self->flags6 |= 0x80;
	STOP_MOVING(self);
	self->fxType = pl->param[0];
	self->countdown = pl->param[1] ? pl->param[1] : 0x3C;
	(*self->cls->method[20])(self, 0xFF);		/* +$56 SetFrame: none */
}
