/* Exodus_StateRiseStart - EXODUS state: remember where it is (targetX/Y, pixels), then rise
 * (Exodus_StateRise).
 *
 * self: the boss. Returns self->flags & 8. */
#include "xmen2.h"

short Exodus_StateRiseStart(register Exodus *self)
{
	self->targetX = self->px;
	self->targetY = self->py;
	SET_STATE(self, Exodus_StateRise);
	return self->flags & 8;
}
