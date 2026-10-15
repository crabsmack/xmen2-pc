/* Exodus_StateSwoopToY - EXODUS state: once within one step (|vy|) of its target height, fire the
 * swoop blast (Exodus_StateSwoopBlast).
 *
 * self: the boss. Returns self->flags & 8 when it switches, no value while waiting. */
#include "xmen2.h"

short Exodus_StateSwoopToY(register Exodus *self)
{
	register long dy;

	dy = (self->targetY << 8) - self->y;
	if (ABS(dy) <= ABS(self->vy)) {
		SET_STATE(self, Exodus_StateSwoopBlast);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
