/* SmallBrood_StateChase - SMALLBROOD state: walk, turning round at the ends of the patrol range,
 * until within 4 pixels of the nearest player's x, then go on with the AI script.
 *
 * self: the object. Returns flags & 8 while walking, nothing when a script step follows. */
#include "xmen2.h"

short SmallBrood_StateChase(SmallBrood *self)
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
	} else if (dx < 4) {
		AI_NEXT_STEP(self, self->ai, 0);
		RETURN_NO_VALUE;
	}
	SET_STATE(self, SmallBrood_StateChase);
	return self->flags & 8;
}
