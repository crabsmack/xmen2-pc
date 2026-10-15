/* BroodBoss_StateWallStop - BROODBOSS state: the wall is in: roar (Tbl_BroodBossAnim3, sound $91),
 * stop the wall and the claw; BroodBoss_StateChargeStart when the animation ends.
 *
 * self: the boss. Returns flags & 8. */
#include "xmen2.h"

short BroodBoss_StateWallStop(BroodBoss *self)
{
	Obj_SetAnim(self, CODEPTR(Tbl_BroodBossAnim3));
	Snd_Stop(0x91);
	Snd_Play(0x91);
	if (self->wall)
		self->wall->vx = 0;
	self->claw->vx = 0;
	self->claw->mode = 0;
	self->claw->motion = 0;
	SET_STATE_WAIT(self, BroodBoss_StateChargeStart, 0xFF);
	return self->flags & 8;
}
