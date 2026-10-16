/* ShootAcolyte_StateNextShot - SHOOTACOLYTE state: after a shot, aim again (ShootAcolyte_StateAim)
 * in shotDelay frames while shots are left; after the last, the next script step at once.
 *
 * self: the object.
 * shotDelay -1 aims again at once (a nested call, whose result it returns). Returns flags bit 3
 * when waiting to aim; no value after the last shot. */
#include "xmen2.h"

short ShootAcolyte_StateNextShot(register ShootAcolyte *self)
{
	if (self->shotsLeft <= 0) {
		AI_NEXT_STEP(self, self->ai, 0);
	} else if (self->shotDelay == -1) {
		self->state = ShootAcolyte_StateAim;
		return ShootAcolyte_StateAim(self);
	} else {
		SET_STATE_WAIT(self, ShootAcolyte_StateAim, self->shotDelay);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
