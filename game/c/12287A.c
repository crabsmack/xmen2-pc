/* BigBio_ActStartArcSwoop - BIGBIO script action: spin the orb ring once, anchor the swoop at the
 * arena side (50 px from the left edge when facing right, else 350; 130 px above the ground
 * position), turn around and swoop (BigBio_StateArcSwoop).
 *
 * self: the boss. Returns flags & 8. Never runs in the corpus. */
#include "xmen2.h"

short BigBio_ActStartArcSwoop(register BigBio *self)
{
	BigBio_UpdateOrbRing(self);
	self->swoopAngle = 0;
	self->targetX = self->facingRight ? (self->arenaLeft + 50) << 8 : (self->arenaLeft + 350) << 8;
	self->facingRight = !self->facingRight;
	self->targetY = (self->py - 130) << 8;
	SET_STATE(self, BigBio_StateArcSwoop);
	return self->flags & 8;
}
