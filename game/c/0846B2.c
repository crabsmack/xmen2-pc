/* SmallBrood_StateLand - SMALLBROOD state: settle on the floor or the ceiling inside the patrol
 * range, then go on with the AI script.
 *
 * self: the object.
 * Standing on its floor or ceiling inside xMin..xMax: upside down on the ceiling, next step.
 * Otherwise: past an x limit it stops there if that side is a wall, else it falls (ay $100,
 * try again next frame); past the floor/ceiling it is put on it and the script restarts at
 * step 0. The v-flip then compares the y from before the move (a ceiling it was put on is
 * not flipped), kept. Returns flags & 8 when it waits, nothing when a script step follows. */
#include "xmen2.h"

short SmallBrood_StateLand(SmallBrood *self)
{
	short x, y;

	x = self->x >> 8;
	y = self->y >> 8;
	(*self->cls->method[20])(self, 0);		/* +$56 SetFrame */
	if ((y == self->floorY || y == self->ceilY) && x >= self->xMin && x <= self->xMax) {
		if (y == self->ceilY)
			self->spr.flags |= 8;
		else
			self->spr.flags &= ~8;
		AI_NEXT_STEP(self, self->ai, 0);
		RETURN_NO_VALUE;
	}
	if (x < self->xMin) {
		if (self->wallLeft) {
			self->x = self->xMin << 8;
			self->ax = 0;
			self->vx = 0;
		} else {
			self->ay = 0x100;
			SET_STATE(self, SmallBrood_StateLand);
			return self->flags & 8;
		}
	} else if (x > self->xMax) {
		if (self->wallRight) {
			self->x = self->xMax << 8;
			self->ax = 0;
			self->vx = 0;
		} else {
			self->ay = 0x100;
			SET_STATE(self, SmallBrood_StateLand);
			return self->flags & 8;
		}
	}
	if (y > self->floorY)
		self->y = self->floorY << 8;
	else if (y < self->ceilY)
		self->y = self->ceilY << 8;
	else {
		self->ay = 0x100;
		SET_STATE(self, SmallBrood_StateLand);
		return self->flags & 8;
	}
	if (y == self->ceilY)
		self->spr.flags |= 8;
	else
		self->spr.flags &= ~8;
	AI_FIRST_STEP(self, self->ai, 0);
	NO_RETURN_VALUE
}
