/* Hologram_Raise_Pose4 - HOLOGRAM attack script, end of steps 2 and 15 (raise: poses 3 4): pose 4, the next step of the script in 10 frames.
 *
 * self: the hologram. */
#include "xmen2.h"

void Hologram_Raise_Pose4(register Hologram *self)
{
	Hologram_SetPose(4);
	AI_NEXT_STEP(self, self->ai, 10);
}
