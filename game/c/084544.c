/* SmallBrood_Move - SMALLBROOD Move (class slot +$4E): the parent's (BIONPC) Move, then keep the
 * object inside its patrol range and between its ceiling and floor.
 *
 * self:  the object.
 * steps: passed on to the parent's Move.
 * An axis that hits a limit is put on it and stops (speed and acceleration 0). */
#include "xmen2.h"

void SmallBrood_Move(SmallBrood *self, short steps)
{
	short x, y;

	(*CLASS(Tbl_SmallBrood_vtable)->parent->method[18])(self, steps);
	x = self->x >> 8;
	y = self->y >> 8;
	if (x < self->xMin) {
		self->x = self->xMin << 8;
		self->ax = 0;
		self->vx = 0;
	} else if (x > self->xMax) {
		self->x = self->xMax << 8;
		self->ax = 0;
		self->vx = 0;
	}
	if (y < self->ceilY) {
		self->y = self->ceilY << 8;
		self->vy = 0;
		self->ay = 0;
	} else if (y > self->floorY) {
		self->y = self->floorY << 8;
		self->vy = 0;
		self->ay = 0;
	}
}
