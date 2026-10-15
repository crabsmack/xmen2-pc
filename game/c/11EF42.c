/* BioBalls_ActClearDone - BIOBALLS script action: not done (BioBalls_IsDone), then the next step
 * at once.
 *
 * self: the object. */
#include "xmen2.h"

void BioBalls_ActClearDone(BioBalls *self)
{
	self->done = 0;
	AI_NEXT_STEP(self, self->ai, 0);
}
