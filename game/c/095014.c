/* EvilDrop_StateBounce2 - EVILDROP state: after the second bounce, fall back to floorY, then stop
 * there (no motion, not falling) and start its AI script at step 0 (EvilDrop_ActOpen first), or
 * its default state without one.
 *
 * self: the capsule.
 * Returns flags & 8 while falling; nothing once landed (state functions' results are not read
 * there). Never runs in the corpus. */
#include "xmen2.h"

short EvilDrop_StateBounce2(EvilDrop *self)
{
	register short y = self->y >> 8;

	if (y >= self->floorY) {
		STOP_MOVING(self);
		self->falling = 0;
		self->y = (long)self->floorY << 8;
		AI_FIRST_STEP(self, self->ai, 0);
	} else {
		SET_STATE(self, EvilDrop_StateBounce2);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
