/* TechWallGun_BurstLevel - TECHWALLGUN state of a wall gun aiming level: a burst straight ahead
 * (direction 2, 5 when h-flipped), shot animation Tbl_Anim_1432AC,
 * fired from the next update (TechWallGun_StateBurst); then back to TechWallGun_StateAimLevel.
 *
 * self: the gun.
 * Returns flags bit 3. */
#include "xmen2.h"

short TechWallGun_BurstLevel(TechWallGun *self)
{
	TechWallGun_SetupBurst(self, CODEPTR(Tbl_Anim_1432AC), TechWallGun_StateAimLevel,
		self->spr.flags & 0x10 ? 5 : 2);
	SET_STATE(self, (StateFn)TechWallGun_StateBurst);
	return self->flags & 8;
}
