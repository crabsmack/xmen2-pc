/* SavageBioBase_Move - SAVAGEBIOBASE Move (class slot +$4E; the parent's isn't called): add the
 * velocity to the position and the acceleration to the velocity, once more for 2 or more steps,
 * then keep it in the arena: x between arenaLeft and arenaRight (stopping in x at an end); its
 * top (its box top, or its y with bioFlags bit 0) not above arenaCeiling, else y not below
 * arenaFloor (either way stopping in y, no gravity).
 *
 * self:  the object.  steps: frames to move (2 in two-player step mode).
 * The pixel position (px, py) isn't updated. */
#include "xmen2.h"

void SavageBioBase_Move(SavageBioBase *self, short steps)
{
	short x, y, top;

	self->x += self->vx;
	self->y += self->vy;
	self->vx += self->ax;
	self->vy += self->ay;
	if (steps > 1) {
		self->x += self->vx;
		self->y += self->vy;
		self->vx += self->ax;
		self->vy += self->ay;
	}
	x = self->x >> 8;
	if (x < self->arenaLeft) {
		self->x = self->arenaLeft << 8;
		self->ax = 0;
		self->vx = 0;
	} else if (x > self->arenaRight) {
		self->x = self->arenaRight << 8;
		self->ax = 0;
		self->vx = 0;
	}
	y = self->y >> 8;
	if (self->bioFlags & 1)
		top = y;
	else
		top = MIN(self->boxTop, self->boxBottom);
	if (top < self->arenaCeiling) {
		self->y = self->arenaCeiling << 8;
		self->vy = 0;
		self->ay = 0;
	} else if (y > self->arenaFloor) {
		self->y = self->arenaFloor << 8;
		self->vy = 0;
		self->ay = 0;
	}
}
