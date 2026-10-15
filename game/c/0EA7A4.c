/* Hologram_ActWaitB - HOLOGRAM attack script action (steps 3 and 16): hold the raised pose: the next step of the script after self->raisedDelay frames
 * (placement parameter 1, default 180).
 *
 * self: the hologram. */
#include "xmen2.h"

void Hologram_ActWaitB(register Hologram *self)
{
	AI_NEXT_STEP(self, self->ai, self->raisedDelay);
}
