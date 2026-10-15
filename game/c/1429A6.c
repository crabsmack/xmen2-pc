/* TechWallGun_StateCeilAimDown - TECHWALLGUN state
 * of a ceiling gun aiming down (frame 8): while it can aim
 * (TechWallGun_AimCeiling), turn right under 68 degrees, left past 112, else fire down
 * (the change on the next update).
 *
 * self: the gun.
 * Returns flags bit 3 when it can aim; nothing in particular otherwise (it keeps the state). */
#include "xmen2.h"

short TechWallGun_StateCeilAimDown(TechWallGun *self)
{
	s16 angle;

	if (TechWallGun_AimCeiling(self, &angle)) {
		(*self->cls->method[20])(self, 8);	/* +$56 SetFrame */
		if (angle < 0x44) {
			SET_STATE(self, TechWallGun_StateCeilAimRight);
			return self->flags & 8;
		}
		if (angle > 0x70) {
			SET_STATE(self, TechWallGun_StateCeilAimLeft);
			return self->flags & 8;
		}
		SET_STATE(self, TechWallGun_BurstCeilDown);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
