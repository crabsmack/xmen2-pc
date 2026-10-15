/* Nightcrawler_StateWaitUpRelease - NIGHTCRAWLER after coming onto a wall: wait for Up to be
 * released, then cling (Nightcrawler_StateWallCling).
 *
 * self: the player.
 * Returns self->flags & 8, as every state. */
#include "xmen2.h"

short Nightcrawler_StateWaitUpRelease(Nightcrawler *self)
{
	if (!self->pad ? g_Pad1Held & g_PadMaskUp : g_Pad2Held & g_PadMaskUp) {
		SET_STATE(self, Nightcrawler_StateWaitUpRelease);
		return self->flags & 8;
	}
	SET_STATE(self, Nightcrawler_StateWallCling);
	return self->flags & 8;
}
