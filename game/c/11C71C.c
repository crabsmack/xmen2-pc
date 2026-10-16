/* BabyBio_ActNext - BABYBIO AI-script action: no animation, no frame shown, next step on the next
 * update.
 *
 * self: the object. */
#include "xmen2.h"

void BabyBio_ActNext(BabyBio *self)
{
	self->anim = 0;
	(*self->cls->method[20])(self, 0xFF);	/* +$56 SetFrame: none */
	AI_NEXT_STEP(self, self->ai, 0);
}
