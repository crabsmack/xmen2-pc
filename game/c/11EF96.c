/* BioBalls_ActSetDone - BIOBALLS script action (BioBalls_SetFormation's next state): no
 * animation, done (BioBalls_IsDone), then the next step on the next update.
 *
 * self: the object. */
#include "xmen2.h"

void BioBalls_ActSetDone(BioBalls *self)
{
	self->anim = 0;
	self->done = 1;
	AI_NEXT_STEP(self, self->ai, 0);
}
