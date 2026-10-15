/* BigBio_StateFlyToTarget - BIGBIO: fly to the target with the orb ring spinning. Closer than $500
 * (Manhattan, 8 fraction bits) it decelerates by 1/16 of its speed per frame and cuts a long
 * countdown to 2; within $100, or when the countdown runs out, it stops and goes on with the
 * script.
 *
 * self: the boss. Returns flags & 8 while flying. Never runs in the corpus. */
#include "xmen2.h"

short BigBio_StateFlyToTarget(register BigBio *self)
{
	long dist;
	long dx, dy;

	dx = self->targetX - self->x;
	dy = self->targetY - self->y;
	dist = ABS(dx) + ABS(dy);
	BigBio_UpdateOrbRing(self);
	if (dist < 0x500) {
		self->ax = -self->vx >> 4;
		self->ay = -self->vy >> 4;
		if (self->countdown > 5)
			self->countdown = 2;
	}
	if (dist > 0x100 && self->countdown) {
		SET_STATE(self, BigBio_StateFlyToTarget);
		return self->flags & 8;
	}
	STOP_MOVING(self);
	AI_NEXT_STEP(self, self->ai, 0);
	NO_RETURN_VALUE
}
