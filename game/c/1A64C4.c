/* Nightcrawler_StateWaitRightRelease - NIGHTCRAWLER after climbing onto a ceiling from a wall on
 * his right: wait for Right to be released, then hang (Nightcrawler_StateCeilingHang).
 *
 * self: the player.
 * Returns self->flags & 8, as every state. */
#include "xmen2.h"

short Nightcrawler_StateWaitRightRelease(Nightcrawler *self)
{
	if (!self->pad ? g_Pad1Held & g_PadMaskRight : g_Pad2Held & g_PadMaskRight) {
		SET_STATE(self, Nightcrawler_StateWaitRightRelease);
		return self->flags & 8;
	}
	SET_STATE(self, Nightcrawler_StateCeilingHang);
	return self->flags & 8;
}
