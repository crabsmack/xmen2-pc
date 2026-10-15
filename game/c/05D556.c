/* Tuskette_StateHopBackRise - TUSKETTE hopper on the way up, hopping back: once it falls
 * (vy > 0), wait to land (Tuskette_StateHopBackLand).
 *
 * self: the object.
 * Returns self->flags & 8 then, nothing while rising. */
#include "xmen2.h"

short Tuskette_StateHopBackRise(Tuskette *self)
{
	if (self->vy > 0) {
		SET_STATE(self, Tuskette_StateHopBackLand);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
