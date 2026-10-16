/* ChildShip_ActWait60 - CHILDSHIP script action: the next step in 60 frames.
 *
 * self: the object (a step of its AI script; never runs in the corpus). */
#include "xmen2.h"

void ChildShip_ActWait60(ChildShip *self)
{
	AI_NEXT_STEP(self, self->ai, 0x3C);
}
