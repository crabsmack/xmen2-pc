/* BigBio_StateArcSwoop - BIGBIO: swoop from the anchor (target) across the arena along an arc,
 * a = 0..90 by 2 per frame: x = anchor + 3.5a px away from the side it started at (315 px in
 * all), y = anchor + 128 px * sin(2a) (down and back up); then the next step of the script.
 *
 * self: the boss. Returns flags & 8 while swooping. Never runs in the corpus. */
#include "xmen2.h"

short BigBio_StateArcSwoop(register BigBio *self)
{
	long dy, dx;

	dy = Math_SinDeg(self->swoopAngle << 1) >> 1;
	dx = ((self->swoopAngle << 2) - (self->swoopAngle >> 1)) << 8;
	if (self->facingRight)
		dx = -dx;
	self->x = self->targetX + dx;
	self->y = self->targetY + dy;
	BigBio_UpdateOrbRing(self);
	self->swoopAngle += 2;
	if (self->swoopAngle > 90) {
		AI_NEXT_STEP(self, self->ai, 0);
	} else {
		SET_STATE(self, BigBio_StateArcSwoop);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
