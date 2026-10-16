/* WallCrawlBio_Move - WALLCRAWLBIO Move (class slot +$4E): crawl, keep it in its crawl range and
 * show it at its home position.
 *
 * self:  the object.
 * steps: not used (the parent's Move isn't called: speed never moves it).
 * On the last frame of its animation's step 3 the home position lunges 7 px the way it moves:
 * x on a ceiling (mode 2), else y. Past an end of the crawl range (xMin / xMax, along the same
 * axis) it is put on it and stops (a closed end: wallLeft / wallRight), or falls
 * (BabyBio_StateFallOff, an open end). Written like BabyBio_Move, gotos included (they give x\n * and y registers of their own, as in the ROM). */
#include "xmen2.h"

void WallCrawlBio_Move(WallCrawlBio *self, short steps)
{
	short x, y;		/* the home position in pixels */

	if (self->mode == 2) {
		if (self->animDist >= ((AnimScript *)self->anim)->delay - 1 && self->animStep == 3) {
			if (self->vx > 0)
				self->homeX += 0x700;
			else if (self->vx < 0)
				self->homeX -= 0x700;
		}
	} else {
		if (self->animDist >= ((AnimScript *)self->anim)->delay - 1 && self->animStep == 3) {
			if (self->vy > 0)
				self->homeY += 0x700;
			else if (self->vy < 0)
				self->homeY -= 0x700;
		}
	}
	if (self->mode == 2) {
		x = self->homeX >> 8;
		if (x < self->xMin) {
			if (self->wallLeft) {
				self->homeX = self->xMin << 8;
				self->ax = 0;
				self->vx = 0;
			} else {
				self->state = (StateFn)BabyBio_StateFallOff;
				self->timer = 0;
				goto show;
			}
		} else if (x > self->xMax) {
			if (self->wallRight) {
				self->homeX = self->xMax << 8;
				self->ax = 0;
				self->vx = 0;
			} else {
				self->state = (StateFn)BabyBio_StateFallOff;
				self->timer = 0;
				goto show;
			}
		}
	} else {
		y = self->homeY >> 8;
		if (y < self->xMin) {
			if (self->wallLeft) {
				self->homeY = self->xMin << 8;
				self->vy = 0;
				self->ay = 0;
			} else {
				self->state = (StateFn)BabyBio_StateFallOff;
				self->timer = 0;
				goto show;
			}
		} else if (y > self->xMax) {
			if (self->wallRight) {
				self->homeY = self->xMax << 8;
				self->vy = 0;
				self->ay = 0;
			} else {
				self->state = (StateFn)BabyBio_StateFallOff;
				self->timer = 0;
				goto show;
			}
		}
	}
show:
	self->x = self->homeX;
	self->y = self->homeY;
}
