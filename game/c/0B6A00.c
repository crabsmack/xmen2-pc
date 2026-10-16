/* BrainChild_Move - BRAINCHILD Move (class slot +$4E; the parent's isn't called): add the
 * velocity to the position and the acceleration to the velocity, once more for 2 or more steps,
 * then keep it in the arena: x between xMin and xMax (stopping in x at an end); its box top not
 * above ceilY (y put so that the box's height hangs from ceilY, stopping in y), else y not below
 * floorY (stopping in y, no gravity).
 *
 * self:  the object.  steps: frames to move (2 in two-player step mode).
 * The pixel position (px, py) isn't updated. */
#include "xmen2.h"

void BrainChild_Move(BrainChild *self, short steps)
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
	if (x < self->xMin) {
		self->x = self->xMin << 8;
		self->ax = 0;
		self->vx = 0;
	} else if (x > self->xMax) {
		self->x = self->xMax << 8;
		self->ax = 0;
		self->vx = 0;
	}
	y = self->y >> 8;
	top = MIN(self->boxTop, self->boxBottom);
	if (top < self->ceilY) {
		self->y = (self->ceilY + ABS(self->boxBottom - self->boxTop)) << 8;
		self->vy = 0;
	} else if (y > self->floorY) {
		self->y = self->floorY << 8;
		self->vy = 0;
		self->ay = 0;
	}
}
