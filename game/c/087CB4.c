/* BroodBoss_StateDeathAnim - BROODBOSS state: the death animation (Tbl_BroodBossAnim8), then
 * BroodBoss_StateDeathFx in 60 frames.
 *
 * self: the boss. Returns flags & 8. */
#include "xmen2.h"

short BroodBoss_StateDeathAnim(BroodBoss *self)
{
	Obj_SetAnim(self, CODEPTR(Tbl_BroodBossAnim8));
	SET_STATE_WAIT(self, BroodBoss_StateDeathFx, 0x3C);
	return self->flags & 8;
}
