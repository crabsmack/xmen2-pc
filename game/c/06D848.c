/* Brood_StateApproach - BROOD state: walk, turning round at the ends of the patrol range, until
 * within 4 pixels of keepDist from the nearest player (x distance), then go on with the AI
 * script.
 *
 * self: the object. Returns flags & 8 while walking, nothing when a script step follows. */
#include "xmen2.h"

short Brood_StateApproach(Brood *self)
{
	XPlayer *p;
	short x, dx;

	p = Player_Nearest((Actor *)self);
	x = self->x >> 8;
	dx = (self->x - p->x) >> 8;
	dx = ABS(dx);
	if (x == self->xMin) {
		self->spr.flags &= ~0x10;
		self->vx = self->speed;
	} else if (x == self->xMax) {
		self->spr.flags |= 0x10;
		self->vx = -self->speed;
	} else if (dx >= self->keepDist - 4 && dx <= self->keepDist + 4) {
		AI_NEXT_STEP(self, self->ai, 0);
		RETURN_NO_VALUE;
	}
	SET_STATE(self, Brood_StateApproach);
	return self->flags & 8;
}
