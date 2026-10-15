/* Tuskette_StateHopInit - TUSKETTE hopper (kind 1), first state: go on with
 * Tuskette_StateHopIdle from the next update.
 *
 * self: the object.
 * Returns self->flags & 8. */
#include "xmen2.h"

short Tuskette_StateHopInit(Tuskette *self)
{
	SET_STATE(self, Tuskette_StateHopIdle);
	return self->flags & 8;
}
