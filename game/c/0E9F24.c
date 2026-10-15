/* Hologram_ExplodeWhenReady - HOLOGRAM dying script, step 3: once its countdown has run out,
 * blow up (Hologram_Explode) and open the exit 30 frames later (Hologram_StateOpenExit).
 *
 * self: the hologram.
 * Returns self->flags & 8 once it explodes, nothing while the countdown runs. */
#include "xmen2.h"

short Hologram_ExplodeWhenReady(register Hologram *self)
{
	if (self->countdown <= 0) {
		Hologram_Explode(self);
		SET_STATE_WAIT(self, Hologram_StateOpenExit, 30);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
