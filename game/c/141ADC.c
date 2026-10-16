/* Tank_ActPause - TANK script action: stop, then the next script step in 30 frames.
 *
 * self: the object. */
#include "xmen2.h"

void Tank_ActPause(register Tank *self)
{
	STOP_MOVING(self);
	AI_NEXT_STEP(self, self->ai, 0x1E);
}
