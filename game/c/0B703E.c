/* BrainChild_ActIdle30 - BRAINCHILD script action: frame 0 (slot +$56), then the next step in 30
 * frames.
 *
 * self: the object (a step of its AI script; never runs in the corpus). */
#include "xmen2.h"

void BrainChild_ActIdle30(BrainChild *self)
{
	(*self->cls->method[20])(self, 0);	/* +$56 SetFrame */
	AI_NEXT_STEP(self, self->ai, 0x1E);
}
