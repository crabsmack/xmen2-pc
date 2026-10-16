/* BioBalls_WaitMotionDone - BIOBALLS state: wait until the motion function is over (it clears
 * itself: BioBalls_MotionGather, _Roll), then the next step of the AI script on the next update.
 *
 * self: the object.
 * Returns flags bit 3 while waiting; nothing in particular after. */
#include "xmen2.h"

short BioBalls_WaitMotionDone(BioBalls *self)
{
	if (self->motion) {
		SET_STATE(self, BioBalls_WaitMotionDone);
		return self->flags & 8;
	}
	AI_NEXT_STEP(self, self->ai, 0);
	NO_RETURN_VALUE
}
