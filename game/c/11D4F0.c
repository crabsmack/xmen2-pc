/* BabyBio_ActDropFlame - BABYBIO AI-script action: kill its flame, next step now.
 *
 * self: the object. */
#include "xmen2.h"

void BabyBio_ActDropFlame(BabyBio *self)
{
	if (self->flame) {
		(*self->flame->cls->method[19])(self->flame);	/* +$52 Kill */
		self->flame = 0;
	}
	AI_NEXT_STEP(self, self->ai, 0);
}
