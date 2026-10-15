/* BigBio_ActJumpToSide - BIGBIO script action: spin the orb ring once and jump to the arena side
 * (50 px from the left edge when facing right, else 350), then gather the orbs back into the
 * shape (BigBio_StateGatherOrbs; countdown 5).
 *
 * self: the boss. Returns flags & 8. Never runs in the corpus. */
#include "xmen2.h"

short BigBio_ActJumpToSide(register BigBio *self)
{
	BigBio_UpdateOrbRing(self);
	self->countdown = 5;
	self->x = self->facingRight ? (self->arenaLeft + 50) << 8 : (self->arenaLeft + 350) << 8;
	self->y = self->py << 8;
	SET_STATE(self, BigBio_StateGatherOrbs);
	return self->flags & 8;
}
