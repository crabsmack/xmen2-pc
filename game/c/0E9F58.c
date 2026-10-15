/* Hologram_StateOpenExit - HOLOGRAM state (30 frames after it exploded): open the exit
 * (Hologram_OpenExit), then wait for the players on the lower lift
 * (Hologram_WaitLowerLiftBoarded2).
 *
 * self: the hologram.
 * Returns self->flags & 8. */
#include "xmen2.h"

short Hologram_StateOpenExit(register Hologram *self)
{
	Hologram_OpenExit(self);
	SET_STATE(self, Hologram_WaitLowerLiftBoarded2);
	return self->flags & 8;
}
