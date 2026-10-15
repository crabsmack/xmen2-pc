/* BabyBio_ActWalkForward - BABYBIO AI-script action: walk at |speed| the way it faces (stop if it
 * has no range, xMin = xMax), next step now.
 *
 * self: the object.
 * The home x in pixels is computed first and never used (kept). Never runs in the corpus. */
#include "xmen2.h"

void BabyBio_ActWalkForward(BabyBio *self)
{
	short x = self->homeX >> 8;

	if (self->xMin == self->xMax)
		STOP_MOVING(self);
	else
		self->vx = (self->spr.flags & 0x10) ? -ABS(self->speed) : ABS(self->speed);
	AI_NEXT_STEP(self, self->ai, 0);
}
