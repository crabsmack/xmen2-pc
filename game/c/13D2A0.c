/* Ninja_StateNextStep - NINJA state (Init's, and its default state): the next AI script step in
 * 60 frames.
 *
 * self: the object. */
#include "xmen2.h"

void Ninja_StateNextStep(register Ninja *self)
{
	AI_NEXT_STEP(self, self->ai, 0x3C);
}
