/* FireWall_StateWaitFlame - FIREWALL state after FireWall_StateSpawnFlame: once it has a flame
 * object with flags bit 4 clear, it forgets it and goes back to FireWall_StateWaitOnScreen
 * from the next update; else it stays the state (with no flame object: for good).
 *
 * self: the object. Returns flags & 8. */
#include "xmen2.h"

short FireWall_StateWaitFlame(FireWall *self)
{
	if (self->flame && !(self->flame->flags & 0x10)) {
		self->flame = 0;
		SET_STATE(self, (StateFn)FireWall_StateWaitOnScreen);
		return self->flags & 8;
	}
	SET_STATE(self, (StateFn)FireWall_StateWaitFlame);
	return self->flags & 8;
}
