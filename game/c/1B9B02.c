/* Psylocke_StateAirDash - Psylocke's psychic air dash (Psylocke_AirSpecial) in progress: it ends,
 * A unlatched, in the control state (slot +$A6), now, once its animation is over or she is on
 * the ground (flags $1000 clear).
 *
 * self: the player.
 * Returns self->flags & 8; nothing when it ends. */
#include "xmen2.h"

short Psylocke_StateAirDash(register Psylocke *self)
{
	if (!self->anim || !(self->flags & 0x1000)) {
		if (!self->pad)
			g_Pad1Latch = g_Pad1Latch & ~g_PadMaskA;
		else
			g_Pad2Latch = g_Pad2Latch & ~g_PadMaskA;
		GOTO_STATE(self, (StateFn)self->cls->method[40]);	/* StateControl */
		RETURN_NO_VALUE;
	}
	SET_STATE(self, (StateFn)Psylocke_StateAirDash);
	return self->flags & 8;
}
