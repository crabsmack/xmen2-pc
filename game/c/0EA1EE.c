/* Hologram_SeqA_Pose0 - HOLOGRAM dying script, end of step 1 (sequence A, poses 1 2 1 0): pose 0, the next step of the script in 10 frames.
 *
 * self: the hologram. */
#include "xmen2.h"

void Hologram_SeqA_Pose0(register Hologram *self)
{
	Hologram_SetPose(0);
	AI_NEXT_STEP(self, self->ai, 10);
}
