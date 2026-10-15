/* BigBio_StateSpitBetween - BIGBIO spitting: mouth frame $85 for one frame, then
 * BigBio_StateSpitFire.
 *
 * self: the boss. Returns flags & 8. */
#include "xmen2.h"

short BigBio_StateSpitBetween(register BigBio *self)
{
	(*self->cls->method[20])(self, 0x85);		/* SetFrame */
	SET_STATE_WAIT(self, BigBio_StateSpitFire, 1);
	return self->flags & 8;
}
