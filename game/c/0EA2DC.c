/* Hologram_SeqB_Pose0 - HOLOGRAM dying script, end of step 2 (sequence B, poses 3 4 3 0): pose 0, the next step of the script in 10 frames.
 *
 * self: the hologram. */
#include "xmen2.h"

void Hologram_SeqB_Pose0(register Hologram *self)
{
	Hologram_SetPose(0);
	AI_NEXT_STEP(self, self->ai, 10);
}
