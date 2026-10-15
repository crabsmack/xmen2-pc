/* Beast_StateDiveHold - diving with B held (Beast_AirAttackDown): the impact on landing
 * (Beast_DiveImpact); B let go or a wall touched (flags $100): the dive goes on unheld
 * (Beast_StateDive).
 *
 * self: the player.
 * Returns self->flags & 8 while in the air; nothing after the impact. */
#include "xmen2.h"

short Beast_StateDiveHold(register Beast *self)
{
	if (!(self->flags & 0x1000)) {
		Beast_DiveImpact(self);
		RETURN_NO_VALUE;
	}
	if (!(!self->pad ? g_Pad1Held & g_PadMaskB : g_Pad2Held & g_PadMaskB) || self->flags & 0x100) {
		SET_STATE(self, (StateFn)Beast_StateDive);
		return self->flags & 8;
	}
	SET_STATE(self, (StateFn)Beast_StateDiveHold);
	return self->flags & 8;
}
