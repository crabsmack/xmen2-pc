/* Nightcrawler_UpdateCharge - NIGHTCRAWLER UpdateCharge (class slot +$76): fill the teleport meter
 * while A is held, twice as fast as XPlayer_UpdateCharge; empty it when A is released.
 *
 * self: the player.
 * Not while teleporting (playerFlags $800000). Below $20 with A held: +2 a frame (+4 in
 * two-player mode). Otherwise A released empties it. Both mark the HUD for redraw. */
#include "xmen2.h"

void Nightcrawler_UpdateCharge(Nightcrawler *self)
{
	if (!(self->playerFlags & 0x800000)
	    && (!self->pad ? g_Pad1Held & g_PadMaskA : g_Pad2Held & g_PadMaskA)
	    && self->charge < 0x20) {
		self->charge += g_TwoPlayerStep ? 4 : 2;
		g_HudDirty = 1;
	} else if (!self->pad ? g_Pad1Released & g_PadMaskA : g_Pad2Released & g_PadMaskA) {
		g_HudDirty = 1;
		self->charge = 0;
	}
}
