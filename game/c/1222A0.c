/* BigBio_StateSpitFire - BIGBIO spitting: open-mouth frame $86 and spit; after the volley's last
 * shot go on with the script, else back to BigBio_StateSpitBetween next frame.
 *
 * self: the boss (shots left in +$B4, from BigBio_ActSpit3).
 * Returns flags & 8 while spitting; nothing in particular when the script goes on. */
#include "xmen2.h"

short BigBio_StateSpitFire(register BigBio *self)
{
	(*self->cls->method[20])(self, 0x86);		/* SetFrame */
	BigBio_SpitSpawn(self, 0);
	if (--self->shotsLeft <= 0) {
		AI_NEXT_STEP(self, self->ai, 0);
	} else {
		SET_STATE_WAIT(self, BigBio_StateSpitBetween, 1);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
