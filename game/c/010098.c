/* BigAcBullet_Attach - hold a new BIGACBULLET at its shooter (BigAcolyte_StateShoot, right after
 * creating it): sprite priority bit set, the direction, the owner and the offset from it.
 *
 * bullet: the BIGACBULLET.  owner: the shooter.  dx, dy: offset in pixels.  dir: direction in
 * degrees (0 right, $B4 left). */
#include "xmen2.h"

void BigAcBullet_Attach(void *bullet, void *owner, short dx, short dy, short dir)
{
	register BigAcBullet *self = (BigAcBullet *)bullet;

	self->spr.flags |= 4;
	self->dir = dir;
	self->owner = (Actor *)owner;
	self->offsetX = (long)dx << 8;
	self->offsetY = (long)dy << 8;
}
