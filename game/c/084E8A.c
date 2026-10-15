/* SmallBrood_StateLanded - SMALLBROOD state after a jump: frame 0, then the next AI-script step.
 *
 * self: the object. */
#include "xmen2.h"

void SmallBrood_StateLanded(SmallBrood *self)
{
	(*self->cls->method[20])(self, 0);		/* +$56 SetFrame */
	AI_NEXT_STEP(self, self->ai, 0);
}
