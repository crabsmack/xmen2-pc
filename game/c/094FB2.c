/* EvilDrop_StateBounce1 - EVILDROP state: after the first bounce, fall back to floorY, then sit on it
 * and bounce up at a quarter of its speed (EvilDrop_StateBounce2).
 *
 * self: the capsule. Returns flags & 8. Never runs in the corpus. */
#include "xmen2.h"

short EvilDrop_StateBounce1(EvilDrop *self)
{
	register short y = self->y >> 8;

	if (y >= self->floorY) {
		self->y = (long)self->floorY << 8;
		self->vy = -(self->vy >> 2);
		SET_STATE(self, EvilDrop_StateBounce2);
		return self->flags & 8;
	} else {
		SET_STATE(self, EvilDrop_StateBounce1);
		return self->flags & 8;
	}
}
