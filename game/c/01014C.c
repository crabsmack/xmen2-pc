/* BigAcBullet_StateFly - BIGACBULLET flying state: the first time, it leaves its owner (snapped to
 * the owner's position plus the offset, then released); each frame it tests the map at its
 * position of before (Tile_TestAt, hook types $7800, hooks run) and is killed (slot +$52) when it
 * hits.
 *
 * self: the bullet.
 * No value returned. */
#include "xmen2.h"

void BigAcBullet_StateFly(register BigAcBullet *self)
{
	register short x, y;

	x = self->x >> 8;
	y = self->y >> 8;
	if (self->owner) {
		self->x = self->owner->x + self->offsetX;
		self->y = self->owner->y + self->offsetY;
		self->owner = 0;
	}
	if ((char)Tile_TestAt(self, x, y, !g_SolidMapIsBg, 0x7800, 1))
		(*self->cls->method[19])(self);			/* +$52 Kill */
}
