/* BrainRocket_StateLaunch - BRAINROCKET's first state (set by BrainRocket_Init): the launch sound
 * ($E9, restarted), h-flipped for a heading between 90 and 270 (exclusive), then
 * BrainRocket_StateSetDirection from the next update.
 *
 * self: the rocket. Returns flags & 8. */
#include "xmen2.h"

short BrainRocket_StateLaunch(BrainRocket *self)
{
	Snd_Stop(0xE9);
	Snd_Play(0xE9);
	if (self->angle > 90 && self->angle < 270)
		self->spr.flags |= 0x10;
	SET_STATE(self, (StateFn)BrainRocket_StateSetDirection);
	return self->flags & 8;
}
