/* BabyBio_ActStartFiring - BABYBIO AI-script action: load a volley (shots) and fire it with
 * BabyBio_ActFireLoop from the next update; countdown 240.
 *
 * self: the object. Returns flags & 8. */
#include "xmen2.h"

short BabyBio_ActStartFiring(BabyBio *self)
{
	self->countdown = 240;
	self->shotsLeft = self->shots;
	SET_STATE(self, BabyBio_ActFireLoop);
	return self->flags & 8;
}
