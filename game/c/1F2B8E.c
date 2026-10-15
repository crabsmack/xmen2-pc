/* XPlayer_AttackRecover - a ground attack is over: unlatch B, remember Up latched (playerFlags
 * $400000), latch the direction still held (Right first) so walking resumes, and the control
 * state (slot +$A6), now.
 *
 * self: the player. */
#include "xmen2.h"

void XPlayer_AttackRecover(XPlayer *self)
{
	if (!self->pad)
		g_Pad1Latch = g_Pad1Latch & ~g_PadMaskB;
	else
		g_Pad2Latch = g_Pad2Latch & ~g_PadMaskB;
	if (!self->pad ? g_Pad1Latch & g_PadMaskUp : g_Pad2Latch & g_PadMaskUp)
		self->playerFlags |= 0x400000;
	if (!self->pad ? g_Pad1Held & g_PadMaskRight : g_Pad2Held & g_PadMaskRight) {
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
