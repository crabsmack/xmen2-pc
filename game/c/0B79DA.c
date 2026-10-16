/* ChildShip_ActWait20 - CHILDSHIP script action: the next step in 20 frames.
 *
 * self: the object (a step of its AI script; never runs in the corpus). */
#include "xmen2.h"

void ChildShip_ActWait20(ChildShip *self)
{
	AI_NEXT_STEP(self, self->ai, 0x14);
}
