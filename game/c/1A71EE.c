/* Nightcrawler_StateDiveHold - NIGHTCRAWLER the dive kick while B is held: landing ends it (normal
 * falling, the control state, slot +$A6, now); B released or pushed in x (flags $100) goes on
 * as Nightcrawler_StateDive from the next update.
 *
 * self: the player.
 * Landing: the record's gravity, air speed and fall speed, no acceleration x, B unlatched, no
 * animation, playerFlags 1 (diving) and 2 (dive used) off.
 * Returns self->flags & 8 while it goes on; nothing on landing. */
#include "xmen2.h"

short Nightcrawler_StateDiveHold(register Nightcrawler *self)
{
	if (!(self->flags & 0x1000)) {
		self->ay = self->charRec->gravity;
		self->ax = 0;
		self->maxVx = self->charRec->airMaxVx;
		self->maxVy = self->charRec->maxFallVy;
		if (!self->pad)
			g_Pad1Latch = g_Pad1Latch & ~g_PadMaskB;
		else
			g_Pad2Latch = g_Pad2Latch & ~g_PadMaskB;
		self->anim = 0;
		self->playerFlags &= ~1;
		self->playerFlags &= ~2;
		GOTO_STATE(self, (StateFn)self->cls->method[40]);	/* StateControl */
		RETURN_NO_VALUE;
	}
	if (!(!self->pad ? g_Pad1Held & g_PadMaskB : g_Pad2Held & g_PadMaskB) || self->flags & 0x100) {
		SET_STATE(self, Nightcrawler_StateDive);
		return self->flags & 8;
	}
	SET_STATE(self, Nightcrawler_StateDiveHold);
	return self->flags & 8;
}
