/* FluxBomb_StateImplode - FLUXBOMB state while it implodes: once the particles are in (phase 2) it
 * detonates (FluxBomb_Detonate from the next frame).
 *
 * self: the bomb.  Returns self->flags & 8. */
#include "xmen2.h"

short FluxBomb_StateImplode(register FluxBomb *self)
{
	if (self->phase == 2) {
		SET_STATE(self, FluxBomb_Detonate);
		return self->flags & 8;
	} else {
		SET_STATE(self, FluxBomb_StateImplode);
		return self->flags & 8;
	}
}
