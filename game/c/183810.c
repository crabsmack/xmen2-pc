/* Beast_RollLand - the forward roll (Beast_StateRoll) lands: stop, uncurl (frame 6, the box
 * centre kept: Beast_SetFrameKeepCenterX), unlatch C, and the control state (slot +$A6), now.
 *
 * self: the player. */
#include "xmen2.h"

void Beast_RollLand(register Beast *self)
{
	STOP_MOVING(self);
	Beast_SetFrameKeepCenterX(self, 6);
	if (!self->pad)
		g_Pad1Latch = g_Pad1Latch & ~g_PadMaskC;
	else
		g_Pad2Latch = g_Pad2Latch & ~g_PadMaskC;
	GOTO_STATE(self, (StateFn)self->cls->method[40]);	/* StateControl */
}
