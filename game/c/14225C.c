/* TechWallGun_FindFreeBullet - a free record (flags state bits 0) of the wall guns' 16 bullets.
 *
 * self: the gun that fires (not read).
 * Returns the first free record; with none free, the address just past the pool (the bug is
 * kept: TechWallGun_Fire then writes a record there). */
#include "xmen2.h"

WallGunBullet *TechWallGun_FindFreeBullet(TechWallGun *self)
{
	WallGunBullet *b = 0;

	for (b = &g_TechWallGunRecs[0]; b < &g_TechWallGunRecs[16]; b++)
		if ((b->flags & 0xF0) == 0)
			return b;
	return b;
}
