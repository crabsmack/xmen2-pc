/* BigBio_StateLaunch - BIGBIO: leap into the air as orbs (BigBio_Launch with the orb shape, body
 * hidden), the ring angles at 60 and 120, 130 px up, sound $FA; then form the orb ring
 * (BigBio_StateFormRing).
 *
 * self: the boss. Returns flags & 8. Never runs in the corpus. */
#include "xmen2.h"

short BigBio_StateLaunch(register BigBio *self)
{
	BigBio_Launch(self, (BigBioShapePt *)CODEPTR(Tbl_BigBioOrbShape));
	(*self->cls->method[20])(self, 0xFF);		/* SetFrame: none */
	self->ax = 0;
	self->vx = 0;
	self->ringSpin = 60;
	self->ringAngle = 120;
	self->py = self->y >> 8;
	self->y -= 0x8200;
	Snd_Stop(0xFA);
	Snd_Play(0xFA);
	SET_STATE(self, BigBio_StateFormRing);
	return self->flags & 8;
}
