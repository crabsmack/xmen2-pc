/* TechWallGun_StateAimUp - TECHWALLGUN state
 * of a wall gun aiming up (frame 0): while it can aim
 * (TechWallGun_AimWall), turn level when the player is above -22 degrees, else fire up
 * (the change on the next update).
 *
 * self: the gun.
 * Returns flags bit 3 when it can aim; nothing in particular otherwise (it keeps the state). */
#include "xmen2.h"

short TechWallGun_StateAimUp(TechWallGun *self)
{
	s16 angle;

	if (TechWallGun_AimWall(self, &angle)) {
		(*self->cls->method[20])(self, 0);	/* +$56 SetFrame */
		if (angle > -22) {
			SET_STATE(self, TechWallGun_StateAimLevel);
			return self->flags & 8;
		}
		SET_STATE(self, TechWallGun_BurstDiagUp);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
