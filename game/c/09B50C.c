/* BroodClaw_StateStartScroll - BROODCLAW state once in view: swing in scroll mode (mode 1: the swing
 * back scrolls the scene) at speed $340 the way it was moving (-$340 unless vx > 0) for $50
 * frames (BroodClaw_StateScroll from the next update).
 *
 * self: the claw.
 * Returns self->flags & 8, as every state. */
#include "xmen2.h"

short BroodClaw_StateStartScroll(BroodClaw *self)
{
	self->vx = self->vx > 0 ? 0x340 : -0x340;
	self->mode = 1;
	self->countdown = 0x50;
	SET_STATE(self, BroodClaw_StateScroll);
	return self->flags & 8;
}
