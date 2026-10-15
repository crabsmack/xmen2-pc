/* TechWallGun_StateCeilAimLeft - TECHWALLGUN state
 * of a ceiling gun aiming down-left (frame 6): while it can
 * aim (TechWallGun_AimCeiling), turn down when the player is under 112 degrees, else fire
 * (the change on the next update).
 *
 * self: the gun.
 * Returns flags bit 3 when it can aim; nothing in particular otherwise (it keeps the state). */
#include "xmen2.h"

short TechWallGun_StateCeilAimLeft(TechWallGun *self)
{
	s16 angle;

	if (TechWallGun_AimCeiling(self, &angle)) {
		(*self->cls->method[20])(self, 6);	/* +$56 SetFrame */
		if (angle < 0x70) {
			SET_STATE(self, TechWallGun_StateCeilAimDown);
			return self->flags & 8;
		}
		SET_STATE(self, TechWallGun_BurstCeilLeft);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
