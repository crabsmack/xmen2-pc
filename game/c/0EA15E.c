/* Hologram_SeqA_Pose1 - HOLOGRAM dying script, step 1 (sequence A): pose 1 for 10 frames, then
 * Hologram_SeqA_Pose2.
 *
 * self: the hologram.
 * Returns self->flags & 8. */
#include "xmen2.h"

short Hologram_SeqA_Pose1(register Hologram *self)
{
	Hologram_SetPose(1);
	SET_STATE_WAIT(self, Hologram_SeqA_Pose2, 10);
	return self->flags & 8;
}
