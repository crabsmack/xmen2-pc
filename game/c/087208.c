/* BroodBoss_ActBringWall - BROODBOSS AI-script action: unless a wall is already there, bring in a
 * BROODBOSSWALL from just off the left edge of the view (camera - 10, + $C9) moving right at
 * $200 and wait 16 frames; then BroodBoss_StateWallStop.
 *
 * self: the boss. Returns flags & 8. */
#include "xmen2.h"

short BroodBoss_ActBringWall(BroodBoss *self)
{
	if (self->wall) {
		SET_STATE(self, BroodBoss_StateWallStop);
		return self->flags & 8;
	}
	self->wall = (BroodBossWall *)(*CLASS(Tbl_BroodBossWall_vtable)->method[1])(0,	/* New */
		CLASS(Tbl_BroodBossWall_vtable), (g_CameraX + -10) << 8, (g_CameraY + 0xC9) << 8);
	if (self->wall)
		self->wall->vx = 0x200;
	SET_STATE_WAIT(self, BroodBoss_StateWallStop, 0x10);
	return self->flags & 8;
}
