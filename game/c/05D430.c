/* Tuskette_StateHopRise - TUSKETTE hopper on the way up: once it falls (vy > 0), wait to land
 * (Tuskette_StateHopLand).
 *
 * self: the object.
 * Returns self->flags & 8 then, nothing while rising. */
#include "xmen2.h"

short Tuskette_StateHopRise(Tuskette *self)
{
	if (self->vy > 0) {
		SET_STATE(self, Tuskette_StateHopLand);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
