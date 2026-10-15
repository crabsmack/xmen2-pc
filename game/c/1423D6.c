/* TechWallGun_StateBurst - TECHWALLGUN state: fire the burst: a shot (Fire, slot +$6E) in the
 * burst's direction every shotDelay frames; after the last, back to the aiming state in
 * burstPause frames.
 *
 * self: the gun. */
#include "xmen2.h"

void TechWallGun_StateBurst(TechWallGun *self)
{
	if (self->shotsLeft-- == 0) {
		self->state = self->aimState;
		self->timer = self->burstPause;
	} else {
		(*self->cls->method[26])(self, self->shotDir);	/* +$6E Fire */
		self->timer = self->shotDelay;
	}
}
