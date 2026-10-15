/* BroodBoss_StateClawOut - BROODBOSS state: the claw swings back in (speed $100, mode 0); then
 * BroodBoss_StateRemoveParts.
 *
 * self: the boss. Returns flags & 8. */
#include "xmen2.h"

short BroodBoss_StateClawOut(BroodBoss *self)
{
	self->claw->vx = 0x100;
	self->claw->mode = 0;
	self->claw->motion = BroodClaw_Swing;
	SET_STATE(self, BroodBoss_StateRemoveParts);
	return self->flags & 8;
}
