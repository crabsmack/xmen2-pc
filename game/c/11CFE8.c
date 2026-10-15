/* BabyBio_ActWalkUntilPlayerAhead - BABYBIO state after BabyBio_ActStartWalk: walk on until the
 * nearest player is ahead (the way it faces) and within 128 px in x of the home position: then
 * the next step now. At an end of its range it goes back to the current step (the script's
 * walk starts again the other way).
 *
 * self: the object. Returns flags & 8 while it walks on (nothing in particular otherwise). */
#include "xmen2.h"

short BabyBio_ActWalkUntilPlayerAhead(BabyBio *self)
{
	short x = self->homeX >> 8;
	XPlayer *p;

	p = Player_Nearest((Actor *)self);
	if (x == self->xMin || x == self->xMax) {
		self->state = FN_AT(StateFn, AI_SCRIPT(self->ai)->step + self->ai.index);
		self->timer = 0;
		RETURN_NO_VALUE;
	}
	if ((self->spr.flags & 0x10) ? p->x < self->x : p->x > self->x) {
		x = (self->homeX - p->x) >> 8;
		x = ABS(x);
		if (x < 0x80) {
			AI_NEXT_STEP(self, self->ai, 0);
			RETURN_NO_VALUE;
		}
	}
	SET_STATE(self, BabyBio_ActWalkUntilPlayerAhead);
	return self->flags & 8;
}
