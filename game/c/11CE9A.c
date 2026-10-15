/* BabyBio_ActMoveHorizontal - BABYBIO AI-script action: walk from one end of its patrol range to
 * the other at |speed| (right from xMin, else left), turned that way, then
 * BabyBio_WaitReachHLimit. A flame now facing the other way is dropped. With no range
 * (xMin = xMax) it stops and takes the next step in 60 frames instead.
 *
 * self: the object. Returns flags & 8 when it walks (nothing in particular otherwise).
 * The home x steps one pixel off the end first, so the wait doesn't end at once. The flame is
 * tested twice before it is killed, as the original did. Never runs in the corpus. */
#include "xmen2.h"

short BabyBio_ActMoveHorizontal(BabyBio *self)
{
	short x = self->homeX >> 8;

	if (self->xMin == self->xMax) {
		STOP_MOVING(self);
		AI_NEXT_STEP(self, self->ai, 60);
	} else {
		self->vy = 0;
		self->ay = 0;
		if (x == self->xMin) {
			self->spr.flags &= ~0x10;
			self->homeX += 0x100;
			self->vx = ABS(self->speed);
		} else {
			self->spr.flags |= 0x10;
			self->homeX -= 0x100;
			self->vx = -ABS(self->speed);
		}
		if (self->flame) {
			if (self->spr.flags & 0x10) {
				if (!(self->flame->spr.flags & 0x10) && self->flame) {
					(*self->flame->cls->method[19])(self->flame);	/* +$52 Kill */
					self->flame = 0;
				}
			} else {
				if ((self->flame->spr.flags & 0x10) && self->flame) {
					(*self->flame->cls->method[19])(self->flame);	/* +$52 Kill */
					self->flame = 0;
				}
			}
		}
		SET_STATE(self, (StateFn)BabyBio_WaitReachHLimit);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
