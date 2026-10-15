/* Hologram_SeqA_Pose1b - HOLOGRAM dying script, step 1 (sequence A): pose 1 for 10 frames, then
 * Hologram_SeqA_Pose0.
 *
 * self: the hologram.
 * Returns self->flags & 8. */
#include "xmen2.h"

short Hologram_SeqA_Pose1b(register Hologram *self)
{
	Hologram_SetPose(1);
	SET_STATE_WAIT(self, (StateFn)Hologram_SeqA_Pose0, 10);
	return self->flags & 8;
}
