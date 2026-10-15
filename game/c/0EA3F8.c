/* Hologram_Lower_Pose0 - HOLOGRAM attack script, end of steps 4 and 17 (lower: poses 3 0): pose 0, the next step of the script in 10 frames.
 *
 * self: the hologram. */
#include "xmen2.h"

void Hologram_Lower_Pose0(register Hologram *self)
{
	Hologram_SetPose(0);
	AI_NEXT_STEP(self, self->ai, 10);
}
