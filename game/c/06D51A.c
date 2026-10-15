/* Brood_StateLand - BROOD state: settle on its floor inside the patrol range, then go on with the
 * AI script.
 *
 * self: the object.
 * On the floor inside xMin..xMax: next step. Otherwise: past an x limit it stops there if that
 * side is a wall, else it falls (ay $100, try again next frame); below the floor or above the
 * ceiling it is put on that line and the next step follows. Returns flags & 8 when it waits,
 * nothing when a script step follows. */
#include "xmen2.h"

short Brood_StateLand(Brood *self)
{
	short x, y;

	x = self->x >> 8;
	y = self->y >> 8;
	(*self->cls->method[20])(self, 0);		/* +$56 SetFrame */
	if (y == self->floorY && x >= self->xMin && x <= self->xMax) {
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
			SET_STATE(self, Brood_StateLand);
			return self->flags & 8;
		}
	} else if (x > self->xMax) {
		if (self->wallRight) {
			self->x = self->xMax << 8;
			self->ax = 0;
			self->vx = 0;
		} else {
			self->ay = 0x100;
			SET_STATE(self, Brood_StateLand);
			return self->flags & 8;
		}
	}
	if (y > self->floorY)
		self->y = self->floorY << 8;
	else if (y < self->ceilY)
		self->y = self->ceilY << 8;
	else {
		self->ay = 0x100;
		SET_STATE(self, Brood_StateLand);
		return self->flags & 8;
	}
	AI_NEXT_STEP(self, self->ai, 0);
	NO_RETURN_VALUE
}
