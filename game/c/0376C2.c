/* MagnetoBoss_Zone4Exit - MAGNETOBOSS leaves camera zone 4: fly to ($7C0, $100) (moveMode 1)
 * and break the wall there (MagnetoBoss_StateBreakWall, also the state to resume after a
 * shot).
 *
 * self: the boss.
 * Returns self->flags & 8. */
#include "xmen2.h"

short MagnetoBoss_Zone4Exit(register MagnetoBoss *self)
{
	self->resumeState = MagnetoBoss_StateBreakWall;
	self->moveMode = 1;
	self->destX = 0x7C0;
	self->destY = 0x100;
	SET_STATE(self, MagnetoBoss_StateBreakWall);
	return self->flags & 8;
}
