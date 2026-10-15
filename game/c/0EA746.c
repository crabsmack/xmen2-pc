/* Hologram_ActPose0WaitA - HOLOGRAM attack script action after each volley (steps 7, 9, 11, 13, 20, 22, 24, 26): pose 0, the next step of the script after self->restDelay frames (placement
 * parameter 0, default 40).
 *
 * self: the hologram. */
#include "xmen2.h"

void Hologram_ActPose0WaitA(register Hologram *self)
{
	Hologram_SetPose(0);
	AI_NEXT_STEP(self, self->ai, self->restDelay);
}
