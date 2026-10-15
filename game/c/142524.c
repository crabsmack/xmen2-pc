/* TechWallGun_BurstCeilLeft - TECHWALLGUN state of a ceiling gun aiming down-left: a burst down-left
 * (direction 7), shot animation Tbl_Anim_1432D0,
 * fired from the next update (TechWallGun_StateBurst); then back to TechWallGun_StateCeilAimLeft.
 *
 * self: the gun.
 * Returns flags bit 3. */
#include "xmen2.h"

short TechWallGun_BurstCeilLeft(TechWallGun *self)
{
	TechWallGun_SetupBurst(self, CODEPTR(Tbl_Anim_1432D0), TechWallGun_StateCeilAimLeft,
		7);
	SET_STATE(self, (StateFn)TechWallGun_StateBurst);
	return self->flags & 8;
}
