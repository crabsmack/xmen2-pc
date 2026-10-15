/* TechWallGun_StartWall - first state of a wall gun (TechWallGun_Init): aim level from the next
 * update (TechWallGun_StateAimLevel).
 *
 * self: the gun.
 * Returns flags bit 3. */
#include "xmen2.h"

short TechWallGun_StartWall(TechWallGun *self)
{
	SET_STATE(self, TechWallGun_StateAimLevel);
	return self->flags & 8;
}
