/* BroodBoss_ActWait30 - BROODBOSS AI-script action: next step in 30 frames.
 *
 * self: the boss. */
#include "xmen2.h"

void BroodBoss_ActWait30(BroodBoss *self)
{
	AI_NEXT_STEP(self, self->ai, 30);
}
