/* CapedAcolyte_ActShootBurst - CAPEDACOLYTE script action: start a burst of burstShots shots
 * (CapedAcolyte_StateShoot from the next update).
 *
 * self: the object.
 * Returns flags bit 3. */
#include "xmen2.h"

short CapedAcolyte_ActShootBurst(register CapedAcolyte *self)
{
	self->shotsLeft = self->burstShots;
	SET_STATE(self, CapedAcolyte_StateShoot);
	return self->flags & 8;
}
