/* XPlayer_ActionPressed - did the player just press something that ends a resting state: attack,
 * jump, a direction, or the special button?
 *
 * self: the player (its pad: +$9D).
 * Returns 1 for B, C or a direction pressed this frame, or A (the special) pressed, or released
 * when the character's special fires on release (playerFlags $10); else 0. Used by the
 * standing and hard-landing states to break out. */
#include "xmen2.h"

char XPlayer_ActionPressed(XPlayer *self)
{
	return !self->pad
		? g_Pad1Pressed & (0xF | g_PadMaskB | g_PadMaskC)
			|| (self->playerFlags & 0x10 ? g_Pad1Released & g_PadMaskA : g_Pad1Pressed & g_PadMaskA)
		: g_Pad2Pressed & (0xF | g_PadMaskB | g_PadMaskC)
			|| (self->playerFlags & 0x10 ? g_Pad2Released & g_PadMaskA : g_Pad2Pressed & g_PadMaskA);
}
