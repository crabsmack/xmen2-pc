/* XPlayer_PushingTetherEdge - two players: is this one pressing toward the side where the tether to
 * its partner holds it back?
 *
 * self: the player.
 * With the partner present and alive: blocked on the right (playerFlags $8000) returns the
 * latched Right bit of its pad, blocked on the left ($4000) the latched Left bit; else 0.
 * The walking and control states then refuse to walk that way. */
#include "xmen2.h"

char XPlayer_PushingTetherEdge(XPlayer *self)
{
	XPlayer *partner;

	partner = self == &g_Player1 ? &g_Player2 : &g_Player1;
	if (!IS_ALIVE(partner))
		return 0;
	if (self->playerFlags & 0x8000)
		return !self->pad ? g_Pad1Latch & g_PadMaskRight : g_Pad2Latch & g_PadMaskRight;
	if (self->playerFlags & 0x4000)
		return !self->pad ? g_Pad1Latch & g_PadMaskLeft : g_Pad2Latch & g_PadMaskLeft;
	return 0;
}
