/* BeamScooter_StateEnter - BEAMSCOOTER first state: frame 0 (its frame-uploaded bit cleared), moving
 * down at $200, then BeamScooter_StateDescend from the next update.
 *
 * self: the scooter. Returns self->flags & 8. */
#include "xmen2.h"

short BeamScooter_StateEnter(register BeamScooter *self)
{
	self->vy = 0x200;
	(*self->cls->method[20])(self, 0);	/* +$56 SetFrame */
	self->spr.flags &= ~0x20;
	SET_STATE(self, BeamScooter_StateDescend);
	return self->flags & 8;
}
