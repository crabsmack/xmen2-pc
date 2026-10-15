/* BigBio_Launch - BIGBIO takes off as orbs: orb mode 8, the orbs on a shape, then a leap up and
 * forward (vx $5BE with drag -27, vy -$420, no gravity, top speeds $1500) for 200 frames, the ring
 * not spinning.
 *
 * self:  the boss.
 * shape: the orb shape, or 0 to keep the orbs where they are. Never runs in the corpus. */
#include "xmen2.h"

void BigBio_Launch(register BigBio *self, register BigBioShapePt *shape)
{
	self->orbMode |= 8;
	if (shape)
		BigBio_SetOrbShape(self, shape);
	self->ax = -27;
	self->vx = 0x5BE;
	self->ay = 0;
	self->vy = -0x420;
	self->maxVx = 0x1500;
	self->maxVy = 0x1500;
	self->countdown = 200;
	self->ringSpeed = 0;
}
