/* Hologram_ActWait30 - HOLOGRAM attack script action (steps 5 and 18): the next step of the script in 30 frames.
 *
 * self: the hologram. */
#include "xmen2.h"

void Hologram_ActWait30(register Hologram *self)
{
	AI_NEXT_STEP(self, self->ai, 30);
}
