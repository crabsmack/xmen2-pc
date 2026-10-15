/* BroodBoss_ActStop - BROODBOSS AI-script action: stop the claw (no speed, mode 0, no motion) and
 * the little leg, next step at once.
 *
 * self: the boss. */
#include "xmen2.h"

void BroodBoss_ActStop(BroodBoss *self)
{
	self->claw->vx = 0;
	self->claw->mode = 0;
	self->claw->motion = 0;
	if (self->leg)
		LittleLeg_SetWalking(self->leg, 0);
	AI_NEXT_STEP(self, self->ai, 0);
}
