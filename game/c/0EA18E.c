/* Hologram_SeqA_Pose2 - HOLOGRAM dying script, step 1 (sequence A): pose 2 for 10 frames, then
 * Hologram_SeqA_Pose1b.
 *
 * self: the hologram.
 * Returns self->flags & 8. */
#include "xmen2.h"

short Hologram_SeqA_Pose2(register Hologram *self)
{
	Hologram_SetPose(2);
	SET_STATE_WAIT(self, Hologram_SeqA_Pose1b, 10);
	return self->flags & 8;
}
