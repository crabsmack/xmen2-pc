/* BigBio_ResumeAfterHit - BIGBIO after the hit blink (BigBio_TakeDamage): stop blinking and go
 * back to the state the hit interrupted (saved in hurtState), if any, on the next update.
 *
 * self: the boss. Returns 0. */
#include "xmen2.h"

short BigBio_ResumeAfterHit(register BigBio *self)
{
	self->flags6 &= ~0x40;
	if (self->hurtState) {
		self->state = self->hurtState;
		self->hurtState = 0;
		self->timer = 0;
	}
	return 0;
}
