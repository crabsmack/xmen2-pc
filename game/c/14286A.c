/* TechWallGun_StateAimLevel - TECHWALLGUN state
 * of a wall gun aiming level (frame 2): while it can aim
 * (TechWallGun_AimWall), turn up below -22 degrees, down past 22, else fire ahead
 * (the change on the next update).
 *
 * self: the gun.
 * Returns flags bit 3 when it can aim; nothing in particular otherwise (it keeps the state). */
#include "xmen2.h"

short TechWallGun_StateAimLevel(TechWallGun *self)
{
	s16 angle;

	if (TechWallGun_AimWall(self, &angle)) {
		(*self->cls->method[20])(self, 2);	/* +$56 SetFrame */
		if (angle < -22) {
			SET_STATE(self, TechWallGun_StateAimUp);
			return self->flags & 8;
		}
		if (angle > 22) {
			SET_STATE(self, TechWallGun_StateAimDown);
			return self->flags & 8;
		}
		SET_STATE(self, TechWallGun_BurstLevel);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
