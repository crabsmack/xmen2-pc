/* Gambit_AirAttack - GAMBIT AirAttack (class slot +$C2): B in the air. Normal gravity; once per
 * jump (playerFlags 2: the kick was used) a fast rise (vy < -$300) is slowed by $100 a frame,
 * then the air kick from the next update (Gambit_AirKick). Landed or kick used: B unlatched and
 * the control state (slot +$A6), now.
 *
 * self: the player. Returns self->flags & 8 when the kick is set, no value otherwise. */
#include "xmen2.h"

short Gambit_AirAttack(register Gambit *self)
{
	register short vy = self->vy;

	self->ay = self->charRec->gravity;
	if (!(self->flags & 0x1000) || self->playerFlags & 2) {
		if (!self->pad)
			g_Pad1Latch = g_Pad1Latch & ~g_PadMaskB;
		else
			g_Pad2Latch = g_Pad2Latch & ~g_PadMaskB;
		GOTO_STATE(self, (StateFn)self->cls->method[40]);	/* StateControl */
	} else if (vy < -0x300)
		self->vy += 0x100;
	else {
		SET_STATE(self, Gambit_AirKick);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
