/* XPlayer_StateAirAttack - an attack in the air: at the animation's end or on landing, unlatch B and
 * A and go back to the control state (slot +$A6) now; meanwhile a wall or ceiling may be grabbed
 * (CheckGrab, slot +$96: then the climbing state, slot +$AA, now).
 *
 * self: the player.
 * Returns self->flags & 8 while it goes on; nothing when it switches. */
#include "xmen2.h"

short XPlayer_StateAirAttack(XPlayer *self)
{
	if (!self->anim || !(self->flags & 0x1000)) {
		if (!self->pad)
			g_Pad1Latch = g_Pad1Latch & ~g_PadMaskB;
		else
			g_Pad2Latch = g_Pad2Latch & ~g_PadMaskB;
		if (!self->pad)
			g_Pad1Latch = g_Pad1Latch & ~g_PadMaskA;
		else
			g_Pad2Latch = g_Pad2Latch & ~g_PadMaskA;
		GOTO_STATE(self, (StateFn)self->cls->method[40]);	/* StateControl */
		RETURN_NO_VALUE;
	}
	if ((char)(*self->cls->method[36])(self)) {			/* CheckGrab */
		GOTO_STATE(self, (StateFn)self->cls->method[41]);	/* StateClimb */
		RETURN_NO_VALUE;
	}
	SET_STATE(self, (StateFn)XPlayer_StateAirAttack);
	return self->flags & 8;
}
