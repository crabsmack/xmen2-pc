/* BigBio_ActFaceSide - BIGBIO script action: face the way +$B6 says, go on with the script on the
 * next update.
 *
 * self: the boss. Never runs in the corpus. */
#include "xmen2.h"

void BigBio_ActFaceSide(register BigBio *self)
{
	if (self->facingRight)
		self->spr.flags &= ~0x10;
	else
		self->spr.flags |= 0x10;
	AI_NEXT_STEP(self, self->ai, 0);
}
