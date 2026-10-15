/* BroodBoss_StateRoar - BROODBOSS state after the setup: roar (Tbl_BroodBossAnim), the body back at
 * boss offset x 0, BroodBoss_StateEnter in 120 frames.
 *
 * self: the boss. Returns flags & 8. */
#include "xmen2.h"

short BroodBoss_StateRoar(BroodBoss *self)
{
	Obj_SetAnim(self, CODEPTR(Tbl_BroodBossAnim));
	g_BroodBossOffsetX = 0;
	SET_STATE_WAIT(self, BroodBoss_StateEnter, 0x78);
	return self->flags & 8;
}
