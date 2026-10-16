/* BroodClaw_StateEnterView - BROODCLAW state: move (speed $340) until it is between $30 and $144 px
 * right of the camera; there stop, install the boss segment hooks ($17 CloneA5_SegHook17, $1A
 * CloneA5_SegHook16) and start the scroll (BroodClaw_StateStartScroll) from the next update.
 *
 * self: the claw (swinging out of scroll mode: mode 0).
 * Returns self->flags & 8, as every state. */
#include "xmen2.h"

short BroodClaw_StateEnterView(BroodClaw *self)
{
	register short x = self->x >> 8;
	register short target;

	self->mode = 0;
	if (x < g_CameraX + 0x30)
		target = g_CameraX + 0x30;
	else if (x > g_CameraX + 0x144)
		target = g_CameraX + 0x144;
	else {
		self->ax = 0;
		self->vx = 0;
		g_SegTypeHooks[0x17] = (SegHookFn)CloneA5_SegHook17;
		g_SegTypeHooks[0x1A] = (SegHookFn)CloneA5_SegHook16;
		SET_STATE(self, BroodClaw_StateStartScroll);
		return self->flags & 8;
	}
	self->vx = x > target ? -0x340 : 0x340;
	SET_STATE(self, BroodClaw_StateEnterView);
	return self->flags & 8;
}
