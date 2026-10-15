/* BigBio_StateSpitStart - BIGBIO: spit sound ($FD), start the volley (BigBio_StateSpitBetween).
 *
 * self: the boss. Returns flags & 8. */
#include "xmen2.h"

short BigBio_StateSpitStart(register BigBio *self)
{
	Snd_Stop(0xFD);
	Snd_Play(0xFD);
	SET_STATE(self, BigBio_StateSpitBetween);
	return self->flags & 8;
}
