/* TechWallGun_Explode - blow up a TECHWALLGUN: an explosion 16 px out from its mounting (right
 * of a left-wall gun, below a ceiling gun, left of a right-wall gun), with its sprite priority,
 * then remove it (Kill).
 *
 * self: the gun.
 * Another mount value would explode at uninitialised coordinates (never happens: placements are
 * 0-2). */
#include "xmen2.h"

void TechWallGun_Explode(TechWallGun *self)
{
	long x, y;

#ifndef XMEN2_MPW
	x = self->x;	/* (C++: defined for a mount the original doesn't handle) */
	y = self->y;
#endif
	switch (self->mount) {
	case 0:
		x = self->x + 0x1000;
		y = self->y;
		break;
	case 1:
		x = self->x;
		y = self->y + 0x2000;
		break;
	case 2:
		x = self->x - 0x1000;
		y = self->y;
		break;
	}
	Explosion_Spawn(x, y, self->spr.flags & 4 ? 1 : 0);
	(*self->cls->method[19])(self);	/* +$52 Kill */
}
