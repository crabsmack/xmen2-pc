/* Hologram_Lower_Pose3 - HOLOGRAM attack script action (steps 4 and 17): lower, pose 3 for 10 frames,
 * then Hologram_Lower_Pose0.
 *
 * self: the hologram.
 * Returns self->flags & 8. */
#include "xmen2.h"

short Hologram_Lower_Pose3(register Hologram *self)
{
	Hologram_SetPose(3);
	SET_STATE_WAIT(self, (StateFn)Hologram_Lower_Pose0, 10);
	return self->flags & 8;
}
