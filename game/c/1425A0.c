/* TechWallGun_BurstCeilRight - TECHWALLGUN state of a ceiling gun aiming down-right: a burst down-right
 * (direction 9), shot animation Tbl_Anim_1432E4,
 * fired from the next update (TechWallGun_StateBurst); then back to TechWallGun_StateCeilAimRight.
 *
 * self: the gun.
 * Returns flags bit 3. */
#include "xmen2.h"

short TechWallGun_BurstCeilRight(TechWallGun *self)
{
	TechWallGun_SetupBurst(self, CODEPTR(Tbl_Anim_1432E4), TechWallGun_StateCeilAimRight,
		9);
	SET_STATE(self, (StateFn)TechWallGun_StateBurst);
	return self->flags & 8;
}
