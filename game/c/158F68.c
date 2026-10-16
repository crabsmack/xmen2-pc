/* FluxBomb_StateBurst - FLUXBOMB state while it bursts: when the burst is over (phase 4) the bomb
 * is killed (class slot +$52).
 *
 * self: the bomb. */
#include "xmen2.h"

void FluxBomb_StateBurst(register FluxBomb *self)
{
	if (self->phase == 4)
		(*self->cls->method[19])(self);			/* +$52 Kill */
}
