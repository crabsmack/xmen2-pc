/* Hologram_SeqB_Pose3b - HOLOGRAM dying script, step 2 (sequence B): pose 3 for 10 frames, then
 * Hologram_SeqB_Pose0.
 *
 * self: the hologram.
 * Returns self->flags & 8. */
#include "xmen2.h"

short Hologram_SeqB_Pose3b(register Hologram *self)
{
	Hologram_SetPose(3);
	SET_STATE_WAIT(self, (StateFn)Hologram_SeqB_Pose0, 10);
	return self->flags & 8;
}
