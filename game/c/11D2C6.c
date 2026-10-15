/* BabyBio_ActFireLoop - BABYBIO state of a volley (BabyBio_ActStartFiring): when the home x is on
 * screen, stop and fire a gauss bullet of its own from 13 px ahead, 31 px up (mirrored by its
 * flips, flying the way it faces, its sprite priority); count the shot either way. Every 16
 * frames until the volley is done (shotsLeft), then the next step after its wait (delay).
 *
 * self: the object. Returns flags & 8 while it fires (nothing in particular after).
 * (The bullet code is SmallBrood_Shoot's.) */
#include "xmen2.h"

short BabyBio_ActFireLoop(BabyBio *self)
{
	short hx = self->homeX >> 8;

	if (hx > g_CameraX && hx < g_CameraX + 0x140) {
		short dx = 13;
		short dy = -31;

		STOP_MOVING(self);
		{
			long x = self->x;
			long y = self->y;
			long flags = (self->spr.flags & 4) ? 0x8000 : 0;

			if (self->spr.flags & 8)
				dy = -dy;
			if (self->spr.flags & 0x10) {
				dx = -dx;
				flags |= 0x800;
			}
			x += dx << 8;
			y += dy << 8;
			flags |= 2;
			GaussBullet_Fire(x, y, self, dx, dy, flags);
		}
	}
	if (--self->shotsLeft <= 0) {
		AI_NEXT_STEP(self, self->ai, self->delay);
	} else {
		SET_STATE_WAIT(self, BabyBio_ActFireLoop, 16);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
