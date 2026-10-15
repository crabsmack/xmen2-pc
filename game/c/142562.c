/* TechWallGun_BurstCeilDown - TECHWALLGUN state of a ceiling gun aiming down: a burst straight down
 * (direction 8), shot animation Tbl_Anim_1432DA,
 * fired from the next update (TechWallGun_StateBurst); then back to TechWallGun_StateCeilAimDown.
 *
 * self: the gun.
 * Returns flags bit 3. */
#include "xmen2.h"

short TechWallGun_BurstCeilDown(TechWallGun *self)
{
	TechWallGun_SetupBurst(self, CODEPTR(Tbl_Anim_1432DA), TechWallGun_StateCeilAimDown,
		8);
	SET_STATE(self, (StateFn)TechWallGun_StateBurst);
	return self->flags & 8;
}
