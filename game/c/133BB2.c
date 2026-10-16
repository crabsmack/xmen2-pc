/* DropRocket_StateLaunch - DROPROCKET's first state (set by DropRocket_Init): the launch sound ($E9,
 * restarted), frame 0, h-flipped for a heading between 90 and 270 (exclusive), and it falls
 * (speed and gravity $300); then DropRocket_StateDrop from the next update.
 *
 * self: the rocket. Returns flags & 8. */
#include "xmen2.h"

short DropRocket_StateLaunch(DropRocket *self)
{
	Snd_Stop(0xE9);
	Snd_Play(0xE9);
	(*self->cls->method[20])(self, 0);		/* +$56 SetFrame */
	if (self->angle > 90 && self->angle < 270)
		self->spr.flags |= 0x10;
	self->vy = 0x300;
	self->ay = 0x300;
	SET_STATE(self, (StateFn)DropRocket_StateDrop);
	return self->flags & 8;
}
