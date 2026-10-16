/* DropRocket_Init - DROPROCKET Init (class slot +$12): the parent's (BRAINROCKET) Init, then state
 * (and default state) DropRocket_StateLaunch.
 *
 * self: the rocket.  x, y, pl: passed to the parent's Init. */
#include "xmen2.h"

void DropRocket_Init(DropRocket *self, long x, long y, Placement *pl)
{
	(*CLASS(Tbl_DropRocket_vtable)->parent->method[3])(self, x, y, pl);	/* BRAINROCKET Init */
	self->state = self->defaultState = (StateFn)DropRocket_StateLaunch;
}
