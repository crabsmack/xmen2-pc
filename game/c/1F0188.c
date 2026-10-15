/* XPlayer_LeaveGround - XPLAYER class slot +$8E, the first frame in the air: normal gravity; after
 * a jump the C button is unlatched, after walking off a ledge the player takes its air frame
 * (XPlayer_EnterAirFrame) and unk_AA = 15.
 *
 * self: the player (playerFlags 8: it jumped).
 * Run by XPlayer_StateControl while playerFlags $200 (on the ground) is still set; clears it,
 * stateTimer, unk_A8 and the x acceleration. */
#include "xmen2.h"

void XPlayer_LeaveGround(XPlayer *self)
{
	self->stateTimer = 0;
	self->unk_A8 = 0;
	self->ax = 0;
	self->ay = self->charRec->gravity;
	if (self->playerFlags & 8) {
		self->unk_AA = 0;
		if (!self->pad)
			g_Pad1Latch = g_Pad1Latch & ~g_PadMaskC;
		else
			g_Pad2Latch = g_Pad2Latch & ~g_PadMaskC;
	} else {
		XPlayer_EnterAirFrame(self);
		self->unk_AA = 15;
	}
	self->playerFlags &= ~0x200;
}
