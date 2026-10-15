/* BigBio_StateDying - BIGBIO dying: the orbs fall, each at its own speed (Tbl_BigBioOrbFallSpeeds,
 * px per frame); when the countdown runs out, Kill (class slot +$52).
 *
 * self: the boss. Returns flags & 8 while falling. Never runs in the corpus.
 * Bug kept: the floor is y >> 32 (0, or -1 when y < 0) instead of a pixel position, so every orb
 * below y 0 is moved off screen (x -200) at once. The ", 0" reproduces the ROM's dead tst.l. */
#include "xmen2.h"

short BigBio_StateDying(register BigBio *self)
{
	register BigBioOrb *orb;
	s16 *fall;
	short floor;
	short i;

	orb = g_BigBioOrbs;
	fall = (s16 *)CODEPTR(Tbl_BigBioOrbFallSpeeds);
#ifdef XMEN2_MPW
	floor = self->y >> 32, 0;
#else
	floor = self->y < 0 ? -1 : 0;	/* the 68000's y >> 32 (C++: undefined) */
#endif
	for (i = 0; i < 16; i++) {
		orb->y += *fall++;
		if (orb->y > floor)
			orb->x = -200;
		orb++;
	}
	if (self->countdown == 0) {
		(*self->cls->method[19])(self);		/* Kill */
	} else {
		SET_STATE(self, BigBio_StateDying);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
