/* Hologram_Fire4_Pose5 - HOLOGRAM attack script action (steps 12 and 19): volley 4, pose 5 for 10 frames,
 * then Hologram_Fire4_Bolts.
 *
 * self: the hologram.
 * Returns self->flags & 8. */
#include "xmen2.h"

short Hologram_Fire4_Pose5(register Hologram *self)
{
	Hologram_SetPose(5);
	SET_STATE_WAIT(self, (StateFn)Hologram_Fire4_Bolts, 10);
	return self->flags & 8;
}
