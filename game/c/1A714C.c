/* Nightcrawler_StateDive - NIGHTCRAWLER the dive kick after B was let go
 * (Nightcrawler_StateDiveHold): it lasts until he lands or the countdown runs out, then normal
 * falling and the control state (slot +$A6), now.
 *
 * self: the player.
 * Ending: the record's gravity, air speed and fall speed, no acceleration x, countdown 0, B
 * unlatched, no animation, playerFlags 1 (diving) off and 2 (dive used) on.
 * Returns self->flags & 8 while it goes on; nothing when it ends. */
#include "xmen2.h"

short Nightcrawler_StateDive(register Nightcrawler *self)
{
	if (!(self->flags & 0x1000) || self->countdown <= 0) {
		self->ay = self->charRec->gravity;
		self->ax = 0;
		self->maxVx = self->charRec->airMaxVx;
		self->maxVy = self->charRec->maxFallVy;
		self->countdown = 0;
		if (!self->pad)
			g_Pad1Latch = g_Pad1Latch & ~g_PadMaskB;
		else
			g_Pad2Latch = g_Pad2Latch & ~g_PadMaskB;
		self->anim = 0;
		self->playerFlags &= ~1;
		self->playerFlags |= 2;
		GOTO_STATE(self, (StateFn)self->cls->method[40]);	/* StateControl */
		RETURN_NO_VALUE;
	}
	SET_STATE(self, Nightcrawler_StateDive);
	return self->flags & 8;
}
