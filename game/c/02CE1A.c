/* Exodus_StateWaitLanded - EXODUS state: once down at its target height, stop moving in y and
 * start the next attack (Exodus_StateNextAttack).
 *
 * self: the boss. Returns self->flags & 8 when it switches, no value while waiting. */
#include "xmen2.h"

short Exodus_StateWaitLanded(register Exodus *self)
{
	if (self->targetY << 8 <= self->y) {
		self->moveFlags &= ~1;
		SET_STATE(self, Exodus_StateNextAttack);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
