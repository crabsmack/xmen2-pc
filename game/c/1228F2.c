/* BigBio_StateWaitThenFlyBack - BIGBIO: spin the orb ring until the countdown runs out, then turn
 * around and fly to the other side (target 50 px from the arena's left edge when now facing
 * right, else 350; 130 px above the ground position) at 1/64 of the distance per frame for 100
 * frames (BigBio_StateFlyToTarget).
 *
 * self: the boss. Returns flags & 8. Never runs in the corpus. */
#include "xmen2.h"

short BigBio_StateWaitThenFlyBack(register BigBio *self)
{
	BigBio_UpdateOrbRing(self);
	if (self->countdown == 0) {
		STOP_MOVING(self);
		self->targetX = (self->facingRight = !self->facingRight) ?
			(self->arenaLeft + 50) << 8 : (self->arenaLeft + 350) << 8;
		self->targetY = (self->py - 130) << 8;
		self->vx = (self->targetX - self->x) >> 6;
		self->vy = (self->targetY - self->y) >> 6;
		self->countdown = 100;
		SET_STATE(self, BigBio_StateFlyToTarget);
		return self->flags & 8;
	}
	SET_STATE(self, BigBio_StateWaitThenFlyBack);
	return self->flags & 8;
}
