/* Tuskette_StateHopBackLand - TUSKETTE hopper coming down after the hop back: once below its
 * home height (py) put it there, flags6 bit $4000 on, and finish (Tuskette_StateHopDone).
 *
 * self: the object.
 * Returns self->flags & 8 then, nothing while falling. */
#include "xmen2.h"

short Tuskette_StateHopBackLand(Tuskette *self)
{
	register long floor;

	floor = self->py;
	floor <<= 8;
	if (floor < self->y) {
		self->y = floor;
		self->flags6 |= 0x4000;
		SET_STATE(self, Tuskette_StateHopDone);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
