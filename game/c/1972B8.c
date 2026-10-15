/* Wolverine_StateDive - the claw dive after B was let go (Wolverine_StateDiveHold): on landing or
 * once countdown runs out, the dive ends in the control state (slot +$A6), now.
 *
 * self: the player.
 * The dive's end: normal gravity, no acceleration x, the air and fall speeds of his record, B
 * unlatched, no animation.
 * Returns self->flags & 8 while diving; nothing after. */
#include "xmen2.h"

short Wolverine_StateDive(register Wolverine *self)
{
	if (!(self->flags & 0x1000) || self->countdown <= 0) {
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
	SET_STATE(self, (StateFn)Wolverine_StateDive);
	return self->flags & 8;
}
