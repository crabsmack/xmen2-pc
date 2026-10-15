/* Tuskette_StateHopLand - TUSKETTE hopper coming down: once below its home height (py, the
 * pixel y it was placed at) put it there, flags6 bit $4000 on, and pause
 * (Tuskette_StateHopPause).
 *
 * self: the object.
 * Returns self->flags & 8 then, nothing while falling. */
#include "xmen2.h"

short Tuskette_StateHopLand(Tuskette *self)
{
	register long floor;

	floor = self->py;
	floor <<= 8;
	if (floor < self->y) {
		self->y = floor;
		self->flags6 |= 0x4000;
		SET_STATE(self, Tuskette_StateHopPause);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
