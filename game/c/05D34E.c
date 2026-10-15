/* Tuskette_StateHopWait - TUSKETTE hopper: when the nearest player is within `range` pixels in
 * x, face it and hop at it 25 frames later (Tuskette_StateHopOut).
 *
 * self: the object.
 * Returns self->flags & 8 then, nothing while waiting. */
#include "xmen2.h"

short Tuskette_StateHopWait(Tuskette *self)
{
	XPlayer *p;

	p = Player_Nearest((Actor *)self);
	if (ABS(p->x - self->x) < self->range << 8) {
		if (p->x < self->x)
			self->spr.flags |= 0x10;
		else
			self->spr.flags &= ~0x10;
		SET_STATE_WAIT(self, Tuskette_StateHopOut, 25);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
