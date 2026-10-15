/* TechWallGun_BurstDiagDown - TECHWALLGUN state of a wall gun aiming down: a burst diagonally down
 * (direction 3, 6 when h-flipped), shot animation Tbl_Anim_1432B6,
 * fired from the next update (TechWallGun_StateBurst); then back to TechWallGun_StateAimDown.
 *
 * self: the gun.
 * Returns flags bit 3. */
#include "xmen2.h"

short TechWallGun_BurstDiagDown(TechWallGun *self)
{
	TechWallGun_SetupBurst(self, CODEPTR(Tbl_Anim_1432B6), TechWallGun_StateAimDown,
		self->spr.flags & 0x10 ? 6 : 3);
	SET_STATE(self, (StateFn)TechWallGun_StateBurst);
	return self->flags & 8;
}
