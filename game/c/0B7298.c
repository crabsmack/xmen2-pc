/* ChildShip_Move - CHILDSHIP Move (class slot +$4E; the parent's isn't called): move its cruise
 * position (x, homeY) by the velocity and the velocity by the acceleration, once per step; keep
 * x between xMin and xMax (stopping in x at an end) and the cruise y between ceilY and floorY
 * (stopping in y, no gravity); then it is shown at the cruise y plus the next bob offset
 * (Tbl_BabyBioBobOffsets, restarted at a dx of $6400 or more).
 *
 * self: the object.  steps: frames to move (2 in two-player step mode; at least one is made).
 * MPW: the loop is a label (a do/while lets x, y and steps share one register; the ROM gives
 * each its own). */
#include "xmen2.h"

void ChildShip_Move(register ChildShip *self, register short steps)
{
	register short x, y;

	self->y = self->homeY;
step:
	self->x += self->vx;
	self->y += self->vy;
	self->vx += self->ax;
	self->vy += self->ay;
	steps--;
	if (steps > 0)
		goto step;
	self->homeY = self->y;
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
	y = self->homeY >> 8;
	if (y < self->ceilY) {
		self->homeY = self->ceilY << 8;
		self->vy = 0;
		self->ay = 0;
	} else if (y > self->floorY) {
		self->homeY = self->floorY << 8;
		self->vy = 0;
		self->ay = 0;
	}
	self->y = self->homeY;
	if (self->bob) {
		if (self->bob->dx >= 0x6400)	/* end marker (100 px): from the start */
			self->bob = (HoverStep *)CODEPTR(Tbl_BabyBioBobOffsets);
		self->y += self->bob->dy;
		self->bob++;
	}
}
