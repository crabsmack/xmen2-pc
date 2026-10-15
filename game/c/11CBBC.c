/* BabyBio_ActWaitPlayerNear192 - BABYBIO AI-script action (and the state it waits in): stop on
 * frame 0 until the nearest player is within 192 px in x of the home position, then the next
 * step now.
 *
 * self: the object. Returns flags & 8 while it waits (nothing in particular after).
 * The home x in pixels is computed first and never used (kept). */
#include "xmen2.h"

short BabyBio_ActWaitPlayerNear192(BabyBio *self)
{
	short dx = self->homeX >> 8;
	XPlayer *p;

	p = Player_Nearest((Actor *)self);
	dx = (self->homeX - p->x) >> 8;
	dx = ABS(dx);
	STOP_MOVING(self);
	self->anim = 0;
	(*self->cls->method[20])(self, 0);	/* +$56 SetFrame */
	if (dx < 0xC0) {
		AI_NEXT_STEP(self, self->ai, 0);
	} else {
		SET_STATE(self, BabyBio_ActWaitPlayerNear192);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
