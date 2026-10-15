/* BigBio_StateBrake - BIGBIO after a charge: spin the orb ring until the countdown runs out, then
 * decelerate (1/16 of the speed per frame) for 16 frames (BigBio_StateWaitThenFlyBack).
 *
 * self: the boss. Returns flags & 8. Never runs in the corpus. */
#include "xmen2.h"

short BigBio_StateBrake(register BigBio *self)
{
	BigBio_UpdateOrbRing(self);
	if (self->countdown == 0) {
		self->ax = -self->vx >> 4;
		self->ay = -self->vy >> 4;
		self->countdown = 16;
		SET_STATE(self, BigBio_StateWaitThenFlyBack);
		return self->flags & 8;
	}
	SET_STATE(self, BigBio_StateBrake);
	return self->flags & 8;
}
