/* SmallBrood_StateNextStep - SMALLBROOD state: go on with the next AI-script step.
 *
 * self: the object. */
#include "xmen2.h"

void SmallBrood_StateNextStep(SmallBrood *self)
{
	AI_NEXT_STEP(self, self->ai, 0);
}
