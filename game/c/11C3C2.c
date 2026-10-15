/* BabyBio_Move - BABYBIO Move (class slot +$4E): move the home position, keep it in the patrol
 * range, and show the object there plus a bob offset.
 *
 * self:  the object.
 * steps: passed on to the parent's (BIONPC) Move.
 * Crawling (Tbl_BabyBioAnim5) it doesn't move by speed: on the last frame of the animation's
 * step 3 the home x lunges 7 px the way it moves. Otherwise the parent's Move moves the home
 * position. Past an end of the range it is put on it and stops in x (a wall), or falls
 * (BabyBio_StateFallOff, an open edge: then y isn't limited this time); y is kept between
 * ceilY and floorY. On frames 5, 6, 8 and 9 the shown position takes the next bob offset
 * (Tbl_BabyBioBobOffsets) on each axis it isn't moving along. */
#include "xmen2.h"

void BabyBio_Move(BabyBio *self, short steps)
{
	short x, y;

	if (self->anim == CODEPTR(Tbl_BabyBioAnim5)) {
		if (self->animDist >= ((AnimScript *)self->anim)->delay - 1 && self->animStep == 3) {
			if (self->vx > 0)
				self->homeX += 0x700;
			else if (self->vx < 0)
				self->homeX -= 0x700;
		}
	} else {
		self->x = self->homeX;
		self->y = self->homeY;
		(*CLASS(Tbl_BabyBio_vtable)->parent->method[18])(self, steps);	/* BIONPC Move */
		self->homeX = self->x;
		self->homeY = self->y;
	}
	x = self->homeX >> 8;
	if (x < self->xMin) {
		if (self->wallLeft) {
			self->homeX = self->xMin << 8;
			self->ax = 0;
			self->vx = 0;
		} else {
			self->state = BabyBio_StateFallOff;
			self->timer = 0;
			goto show;
		}
	} else if (x > self->xMax) {
		if (self->wallRight) {
			self->homeX = self->xMax << 8;
			self->ax = 0;
			self->vx = 0;
		} else {
			self->state = BabyBio_StateFallOff;
			self->timer = 0;
			goto show;
		}
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
show:
	self->x = self->homeX;
	self->y = self->homeY;
	if (self->spr.frame == 5 || self->spr.frame == 6 || self->spr.frame == 8
	    || self->spr.frame == 9) {
		if (self->bob->dx >= 0x6400)
			self->bob = (HoverStep *)CODEPTR(Tbl_BabyBioBobOffsets);
		if (!self->vx)
			self->x += self->bob->dx;
		if (!self->vy)
			self->y += self->bob->dy;
		self->bob++;
	}
}
