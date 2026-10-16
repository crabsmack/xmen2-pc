/* BroodClaw_StateScroll - BROODCLAW state while the boss scroll runs (BroodClaw_StateStartScroll):
 * when its countdown is out the scroll is over (scrollEnded: BroodClaw_Swing hands the camera
 * back); once the swing has left scroll mode (mode 0), drop segment hook $17 and move to the drop
 * point (BroodClaw_StateMoveToDrop) from the next update.
 *
 * self: the claw.
 * Returns self->flags & 8, as every state. */
#include "xmen2.h"

short BroodClaw_StateScroll(BroodClaw *self)
{
	if (self->countdown <= 0) {
		self->scrollEnded = 1;
		if (!self->mode) {
			g_SegTypeHooks[0x17] = 0;
			SET_STATE(self, BroodClaw_StateMoveToDrop);
			return self->flags & 8;
		}
	}
	SET_STATE(self, BroodClaw_StateScroll);
	return self->flags & 8;
}
