/* XClone_StateLand - XCLONE state after a hit: fall (gravity $C0 unless it already has one) until
 * it stands on the arena floor, then end the hit flash and start its script from the first step.
 *
 * self: the clone (state set by XClone_OnHurt).
 * Returns flags & 8 while falling; nothing in particular once landed. */
#include "xmen2.h"

short XClone_StateLand(XClone *self)
{
	if (self->arenaFloor << 8 > self->y) {
		if (!self->ay)
			self->ay = 0xC0;
		SET_STATE(self, XClone_StateLand);
		return self->flags & 8;
	}
	self->y = self->arenaFloor << 8;
	self->flags6 &= ~0x4000;
	AI_FIRST_STEP(self, self->ai, 0);
	NO_RETURN_VALUE
}
