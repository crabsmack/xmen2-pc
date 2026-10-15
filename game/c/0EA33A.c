/* Hologram_Raise_Pose3 - HOLOGRAM attack script action (steps 2 and 15): raise, pose 3 for 10 frames,
 * then Hologram_Raise_Pose4.
 *
 * self: the hologram.
 * Returns self->flags & 8. */
#include "xmen2.h"

short Hologram_Raise_Pose3(register Hologram *self)
{
	Hologram_SetPose(3);
	SET_STATE_WAIT(self, (StateFn)Hologram_Raise_Pose4, 10);
	return self->flags & 8;
}
