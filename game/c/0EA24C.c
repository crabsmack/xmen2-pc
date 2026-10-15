/* Hologram_SeqB_Pose3 - HOLOGRAM dying script, step 2 (sequence B): pose 3 for 10 frames, then
 * Hologram_SeqB_Pose4.
 *
 * self: the hologram.
 * Returns self->flags & 8. */
#include "xmen2.h"

short Hologram_SeqB_Pose3(register Hologram *self)
{
	Hologram_SetPose(3);
	SET_STATE_WAIT(self, Hologram_SeqB_Pose4, 10);
	return self->flags & 8;
}
