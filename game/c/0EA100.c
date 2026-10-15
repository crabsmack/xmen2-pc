/* Hologram_ActPose0 - HOLOGRAM attack script action (steps 1 and 14): pose 0, the next step of
 * the script in 10 frames.
 *
 * self: the hologram. */
#include "xmen2.h"

void Hologram_ActPose0(register Hologram *self)
{
	Hologram_SetPose(0);
	AI_NEXT_STEP(self, self->ai, 10);
}
