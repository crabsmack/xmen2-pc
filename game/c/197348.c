/* Wolverine_StateDiveHold - the claw dive with B held (Wolverine_AirAttackDown): landing ends it in
 * the control state (slot +$A6), now; B let go or a wall touched (flags $100): the dive goes on
 * unheld (Wolverine_StateDive).
 *
 * self: the player.
 * On landing: normal gravity, no acceleration x, the air and fall speeds of his record, B
 * unlatched, no animation.
 * Returns self->flags & 8 while in the air; nothing after landing. */
#include "xmen2.h"

short Wolverine_StateDiveHold(register Wolverine *self)
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
		GOTO_STATE(self, (StateFn)self->cls->method[40]);	/* StateControl */
		RETURN_NO_VALUE;
	}
	if (!(!self->pad ? g_Pad1Held & g_PadMaskB : g_Pad2Held & g_PadMaskB) || self->flags & 0x100) {
		SET_STATE(self, (StateFn)Wolverine_StateDive);
		return self->flags & 8;
	}
	SET_STATE(self, (StateFn)Wolverine_StateDiveHold);
	return self->flags & 8;
}
