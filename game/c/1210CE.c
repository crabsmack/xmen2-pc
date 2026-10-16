/* BioSentinel_StateAfterSwipe - BIOSENTINEL after a swipe: think again (BioSentinel_StateThink),
 * at once when its pause is -1, else after the pause.
 *
 * self: the sentinel. Returns self->flags & 8, or what BioSentinel_StateThink returns. */
#include "xmen2.h"

short BioSentinel_StateAfterSwipe(register BioSentinel *self)
{
	if (self->pause == -1) {
		self->state = BioSentinel_StateThink;
		return BioSentinel_StateThink(self);
	}
	SET_STATE_WAIT(self, BioSentinel_StateThink, self->pause);
	return self->flags & 8;
}
