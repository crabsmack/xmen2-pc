/* Hologram_StateStart - HOLOGRAM's first state (Hologram_Init): Hologram_StateSetup in 10
 * frames.
 *
 * self: the hologram.
 * Returns self->flags & 8. */
#include "xmen2.h"

short Hologram_StateStart(register Hologram *self)
{
	SET_STATE_WAIT(self, Hologram_StateSetup, 10);
	return self->flags & 8;
}
