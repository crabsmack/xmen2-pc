/* Hologram_Fire1_Pose6 - HOLOGRAM attack script action (steps 6 and 25): volley 1, pose 6 for 10 frames,
 * then Hologram_Fire1_Bolts.
 *
 * self: the hologram.
 * Returns self->flags & 8. */
#include "xmen2.h"

short Hologram_Fire1_Pose6(register Hologram *self)
{
	Hologram_SetPose(6);
	SET_STATE_WAIT(self, (StateFn)Hologram_Fire1_Bolts, 10);
	return self->flags & 8;
}
