/* BabyBio_StateStart - BABYBIO's first state (set by BabyBio_Init): no frame shown, then the AI
 * script from its first step now (no script: the default state).
 *
 * self: the object. */
#include "xmen2.h"

void BabyBio_StateStart(BabyBio *self)
{
	(*self->cls->method[20])(self, 0xFF);	/* +$56 SetFrame: none */
	AI_FIRST_STEP(self, self->ai, 0);
}
