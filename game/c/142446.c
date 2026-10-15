/* TechWallGun_BurstDiagUp - TECHWALLGUN state of a wall gun aiming up: a burst diagonally up
 * (direction 1, 4 when h-flipped), shot animation Tbl_Anim_1432A2,
 * fired from the next update (TechWallGun_StateBurst); then back to TechWallGun_StateAimUp.
 *
 * self: the gun.
 * Returns flags bit 3. */
#include "xmen2.h"

short TechWallGun_BurstDiagUp(TechWallGun *self)
{
	TechWallGun_SetupBurst(self, CODEPTR(Tbl_Anim_1432A2), TechWallGun_StateAimUp,
		self->spr.flags & 0x10 ? 4 : 1);
	SET_STATE(self, (StateFn)TechWallGun_StateBurst);
	return self->flags & 8;
}
