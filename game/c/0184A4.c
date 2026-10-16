/* ShootAcolyte_ActShootBurst - SHOOTACOLYTE script action: start a burst of burstShots shots
 * (ShootAcolyte_StateAim from the next update).
 *
 * self: the object.
 * Returns flags bit 3. */
#include "xmen2.h"

short ShootAcolyte_ActShootBurst(register ShootAcolyte *self)
{
	self->shotsLeft = self->burstShots;
	SET_STATE(self, ShootAcolyte_StateAim);
	return self->flags & 8;
}
