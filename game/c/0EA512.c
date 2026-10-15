/* Hologram_Fire2_Pose6 - HOLOGRAM attack script action (steps 8 and 23): volley 2, pose 6 for 10 frames,
 * then Hologram_Fire2_Bolts.
 *
 * self: the hologram.
 * Returns self->flags & 8. */
#include "xmen2.h"

short Hologram_Fire2_Pose6(register Hologram *self)
{
	Hologram_SetPose(6);
	SET_STATE_WAIT(self, (StateFn)Hologram_Fire2_Bolts, 10);
	return self->flags & 8;
}
