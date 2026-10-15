/* BabyBio_ActWaitPlayerNear128 - BABYBIO AI-script action (and the state it waits in): stop and
 * keep facing the nearest player until it is within 128 px in x of the home position, then the
 * next step now.
 *
 * self: the object. Returns flags & 8 while it waits (nothing in particular after).
 * The home x in pixels is computed first and never used (kept). */
#include "xmen2.h"

short BabyBio_ActWaitPlayerNear128(BabyBio *self)
{
	short dx = self->homeX >> 8;
	XPlayer *p;

	STOP_MOVING(self);
	p = Player_Nearest((Actor *)self);
	dx = (self->homeX - p->x) >> 8;
	dx = ABS(dx);
	if (p->x < self->x)
		self->spr.flags |= 0x10;
	else
		self->spr.flags &= ~0x10;
	if (dx < 0x80) {
		AI_NEXT_STEP(self, self->ai, 0);
	} else {
		SET_STATE(self, BabyBio_ActWaitPlayerNear128);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
