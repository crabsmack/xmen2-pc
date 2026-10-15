/* Hologram_SeqB_Pose4 - HOLOGRAM dying script, step 2 (sequence B): pose 4 for 10 frames, then
 * Hologram_SeqB_Pose3b.
 *
 * self: the hologram.
 * Returns self->flags & 8. */
#include "xmen2.h"

short Hologram_SeqB_Pose4(register Hologram *self)
{
	Hologram_SetPose(4);
	SET_STATE_WAIT(self, Hologram_SeqB_Pose3b, 10);
	return self->flags & 8;
}
