/* SmallBrood_ActIdle - SMALLBROOD AI-script action: stand still on frame 0, next step in 60 frames.
 *
 * self: the object. */
#include "xmen2.h"

void SmallBrood_ActIdle(SmallBrood *self)
{
	self->anim = 0;
	(*self->cls->method[20])(self, 0);		/* +$56 SetFrame */
	STOP_MOVING(self);
	AI_NEXT_STEP(self, self->ai, 60);
}
