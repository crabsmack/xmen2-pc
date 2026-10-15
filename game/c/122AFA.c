/* BigBio_StateSpinUp - BIGBIO: spin the orb ring up; while it is still slow the orb tiles switch
 * between frame 1 (18 frames) and frame 2 (200 frames); once up to speed, the next step of the
 * script.
 *
 * self: the boss. Returns flags & 8 while spinning up. Never runs in the corpus. */
#include "xmen2.h"

short BigBio_StateSpinUp(register BigBio *self)
{
	if (BigBio_UpdateOrbRing(self)) {
		AI_NEXT_STEP(self, self->ai, 0);
	} else {
		if (self->countdown == 0) {
			if (self->orbFrame != 1) {
				self->orbFrame = 1;
				self->countdown = 18;
			} else {
				self->orbFrame = 2;
				self->countdown = 200;
			}
		}
		SET_STATE(self, BigBio_StateSpinUp);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
