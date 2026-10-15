/* XPlayer_CrouchAttackRecover - a crouching attack is over: unlatch B, stay crouched (playerFlags
 * $200000); Down held unlatches Left/Right, else the direction still held is latched (Right
 * first); then the control state (slot +$A6), now.
 *
 * self: the player. */
#include "xmen2.h"

void XPlayer_CrouchAttackRecover(XPlayer *self)
{
	if (!self->pad)
		g_Pad1Latch = g_Pad1Latch & ~g_PadMaskB;
	else
		g_Pad2Latch = g_Pad2Latch & ~g_PadMaskB;
	self->playerFlags |= 0x200000;
	if (!self->pad ? g_Pad1Held & g_PadMaskDown : g_Pad2Held & g_PadMaskDown) {
		if (!self->pad)
			g_Pad1Latch = g_Pad1Latch & ~g_PadMaskLeftRight;
		else
			g_Pad2Latch = g_Pad2Latch & ~g_PadMaskLeftRight;
	} else if (!self->pad ? g_Pad1Held & g_PadMaskRight : g_Pad2Held & g_PadMaskRight) {
		if (!self->pad)
			g_Pad1Latch |= g_PadMaskRight;
		else
			g_Pad2Latch |= g_PadMaskRight;
	} else if (!self->pad ? g_Pad1Held & g_PadMaskLeft : g_Pad2Held & g_PadMaskLeft) {
		if (!self->pad)
			g_Pad1Latch |= g_PadMaskLeft;
		else
			g_Pad2Latch |= g_PadMaskLeft;
	}
	GOTO_STATE(self, (StateFn)self->cls->method[40]);	/* StateControl */
}
