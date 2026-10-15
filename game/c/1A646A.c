/* Nightcrawler_StateWaitLeftRelease - NIGHTCRAWLER after climbing onto a ceiling from a wall on
 * his left: wait for Left to be released, then hang (Nightcrawler_StateCeilingHang).
 *
 * self: the player.
 * Returns self->flags & 8, as every state. */
#include "xmen2.h"

short Nightcrawler_StateWaitLeftRelease(Nightcrawler *self)
{
	if (!self->pad ? g_Pad1Held & g_PadMaskLeft : g_Pad2Held & g_PadMaskLeft) {
		SET_STATE(self, Nightcrawler_StateWaitLeftRelease);
		return self->flags & 8;
	}
	SET_STATE(self, Nightcrawler_StateCeilingHang);
	return self->flags & 8;
}
