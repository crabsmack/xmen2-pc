/* XPlayer_StateHurtRecover - knocked back after a hit (XPlayer_StateHurt): wait for the landing or,
 * in the air, the end of the hurt time; then blink unhittable for 40 frames (flags6 $40) and
 * latch the direction still held (Right first).
 *
 * self: the player (2 frames unhittable while it waits).
 * Landing: no longer jumped (playerFlags 8), Land (slot +$9A), then a hard landing (slot +$EA)
 * from a fall speed of $44C on, else stopped and the control state (slot +$A6), now. The hurt
 * time over in the air: the control state now.
 * Returns self->flags & 8 while it waits; nothing when it switches. */
#include "xmen2.h"

short XPlayer_StateHurtRecover(XPlayer *self)
{
	self->hitCooldown = 2;
	if (!(self->flags & 0x1000)) {
		self->hitCooldown = 40;
		self->flags6 |= 0x40;
		self->playerFlags &= ~8;
		(*self->cls->method[37])(self);			/* Land */
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
		if (self->prevVy >= 0x44C)
			GOTO_STATE(self, (StateFn)self->cls->method[57]);	/* HardLanding */
		else {
			STOP_MOVING(self);
			GOTO_STATE(self, (StateFn)self->cls->method[40]);	/* StateControl */
		}
		RETURN_NO_VALUE;
	}
	if (self->countdown <= 0) {
		self->hitCooldown = 40;
		self->flags6 |= 0x40;
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
		GOTO_STATE(self, (StateFn)self->cls->method[40]);		/* StateControl */
		RETURN_NO_VALUE;
	}
	SET_STATE(self, (StateFn)XPlayer_StateHurtRecover);
	return self->flags & 8;
}
