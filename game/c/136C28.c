/* Hornet_StateFire - HORNET state: fire a gauss bullet ahead (8 px in front, 2 px down, speed 8
 * and 2 the way it faces), play the firing animation and recover when it ends
 * (Hornet_StateShotRecover).
 *
 * self: the hornet. Returns self->flags & 8.
 * GaussBullet_Fire flags: 3, | $800 facing left, | $8000 with the sprite's priority bit. */
#include "xmen2.h"

short Hornet_StateFire(Hornet *self)
{
	long x = self->x;
	long y = self->y;
	short dx = 8;
	short dy = 2;
	long flags = self->spr.flags & 4 ? 0x8000 : 0;

	if (self->spr.flags & 0x10) {
		dx = -dx;
		flags |= 0x800;
	}
	x += dx << 8;
	y += dy << 8;
	flags |= 3;
	GaussBullet_Fire(x, y, self, dx, dy, flags);
	Obj_ChangeAnim(self, CODEPTR(Tbl_HornetAnimFire));
	SET_STATE_WAIT(self, (StateFn)Hornet_StateShotRecover, 0xFF);
	return self->flags & 8;
}
