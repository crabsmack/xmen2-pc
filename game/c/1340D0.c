/* FireWall_StateWaitOnScreen - FIREWALL state: once its x is in the screen's width (g_CameraX to
 * g_CameraX + 320), FireWall_StateSpawnFlame from the next update; else it stays the state.
 *
 * self: the object. Returns flags & 8. */
#include "xmen2.h"

short FireWall_StateWaitOnScreen(FireWall *self)
{
	register short x = self->x >> 8;

	if (x >= g_CameraX && x < g_CameraX + 0x140) {
		SET_STATE(self, (StateFn)FireWall_StateSpawnFlame);
		return self->flags & 8;
	}
	SET_STATE(self, (StateFn)FireWall_StateWaitOnScreen);
	return self->flags & 8;
}
