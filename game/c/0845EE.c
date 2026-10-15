/* SmallBrood_Shoot - fire a gauss bullet from an offset of the object (class slot +$82).
 *
 * self:   the object (SMALLBROOD, BROOD).
 * dx, dy: offset in pixels, mirrored by the sprite's h-flip and v-flip.
 * mine:   nonzero: the bullet belongs to self (owner self, flag 2); 0: no owner.
 * The bullet flies the way the offset points ($800: to the left) and takes the sprite's
 * priority bit ($8000). (The flags ?: is an initializer: MPW C types an assigned ?: of
 * constants by their values, $8000 as unsigned short, but an initializer as the variable.) */
#include "xmen2.h"

void SmallBrood_Shoot(SmallBrood *self, short dx, short dy, char mine)
{
	long x = self->x;
	long y = self->y;
	long flags = (self->spr.flags & 4) ? 0x8000 : 0;
	SmallBrood *owner = 0;

	if (self->spr.flags & 8)
		dy = -dy;
	if (self->spr.flags & 0x10) {
		dx = -dx;
		flags |= 0x800;
	}
	x += dx << 8;
	y += dy << 8;
	if (mine) {
		owner = self;
		flags |= 2;
	}
	GaussBullet_Fire(x, y, owner, dx, dy, flags);
}
