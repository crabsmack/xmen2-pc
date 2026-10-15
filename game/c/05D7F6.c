/* Tuskette_StateWaitCharge - TUSKETTE runner: when the nearest player stands on its floor
 * segment within `range` pixels in x, remember the player, face it and run at it
 * (Tuskette_StateRunRight / Tuskette_StateRunLeft).
 *
 * self: the object.
 * Returns self->flags & 8 then, nothing while waiting. */
#include "xmen2.h"

short Tuskette_StateWaitCharge(Tuskette *self)
{
	XPlayer *p;

	p = Player_Nearest((Actor *)self);
	if (p->seg == self->seg && ABS(p->x - self->x) < self->range << 8) {
		self->player = p;
		if (p->x < self->x)
			self->spr.flags |= 0x10;
		else
			self->spr.flags &= ~0x10;
		if (!(self->spr.flags & 0x10)) {
			SET_STATE(self, Tuskette_StateRunRight);
			return self->flags & 8;
		} else {
			SET_STATE(self, Tuskette_StateRunLeft);
			return self->flags & 8;
		}
	}
	NO_RETURN_VALUE
}
