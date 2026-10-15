/* Exodus_StateWaitBlastsGone - EXODUS state: wait until both sway angles are back at 0 (the sway
 * of Exodus_StateVulnerable has come full circle: every 72 x steps), then stop swaying and
 * start the next attack (Exodus_StateNextAttack).
 *
 * self: the boss. Returns self->flags & 8 when it switches, no value while waiting.
 * (The disassembly's name guessed +$98/+$9A were blast counters: they are the sway angles.) */
#include "xmen2.h"

short Exodus_StateWaitBlastsGone(register Exodus *self)
{
	if (!self->swayAngleX && !self->swayAngleY) {
		self->moveFlags &= ~0xC;
		SET_STATE(self, Exodus_StateNextAttack);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
