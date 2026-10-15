/* BroodBoss_ActWait60 - BROODBOSS AI-script action: next step in 60 frames.
 *
 * self: the boss. */
#include "xmen2.h"

void BroodBoss_ActWait60(BroodBoss *self)
{
	AI_NEXT_STEP(self, self->ai, 60);
}
