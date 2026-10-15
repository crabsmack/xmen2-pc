/* XPlayer_UpdateCharge - XPLAYER class slot +$76: fill the special meter while A is held, empty it
 * when A is released.
 *
 * self: the player.
 * Below $20 and without playerFlags $2000, A held adds 1 a frame (2 in two-player mode: the
 * logic runs at half rate); reaching $20 sets playerFlags $800 (meter full). Otherwise A
 * released empties it. Both mark the HUD for redraw. */
#include "xmen2.h"

void XPlayer_UpdateCharge(XPlayer *self)
{
	if (self->charge < 0x20 && !(self->playerFlags & 0x2000)
	    && (!self->pad ? g_Pad1Latch & g_PadMaskA : g_Pad2Latch & g_PadMaskA)) {
		g_HudDirty = 1;
		self->charge += g_TwoPlayerStep ? 2 : 1;
		if (self->charge >= 0x20)
			self->playerFlags |= 0x800;
	} else if (!self->pad ? g_Pad1Released & g_PadMaskA : g_Pad2Released & g_PadMaskA) {
		g_HudDirty = 1;
		self->charge = 0;
	}
}
