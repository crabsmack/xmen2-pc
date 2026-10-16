/* SavageMediumBio_StateFire - SAVAGEMEDIUMBIO state: fire a gauss bullet (GaussBullet_Fire) with
 * sound $EE (restarted), then go on with the script on the next update.
 *
 * self: the object (after SavageMediumBio_ActShoot's animation).
 * The muzzle offset is $36 px ahead and $34 above (below when v-flipped; mirrored and flag $800
 * when facing left); flags $8000 with the sprite's priority bit, | $A. The bullet's position is
 * passed as that offset << 8 alone, not added to the object's position (it starts near the
 * top left of the map): kept. Returns nothing in particular. Never runs in the corpus. */
#include "xmen2.h"

short SavageMediumBio_StateFire(register SavageMediumBio *self)
{
	long bx = self->x;
	register long by = self->y;
	register short dx = 0x36;
	register short dy = -0x34;
	register long flags = self->spr.flags & 4 ? 0x8000 : 0;

	if (self->spr.flags & 8)
		dy = -dy;
	if (self->spr.flags & 0x10) {
		dx = -dx;
		flags |= 0x800;
	}
	bx = (long)dx << 8;
	by = (long)dy << 8;
	flags |= 0xA;
	GaussBullet_Fire(bx, by, self, dx, dy, flags);
	Snd_Stop(0xEE);
	Snd_Play(0xEE);
	AI_NEXT_STEP(self, self->ai, 0);
	NO_RETURN_VALUE
}
