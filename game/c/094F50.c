/* EvilDrop_StateFall - EVILDROP state: fall until it reaches floorY, then sit on it and bounce up at
 * half its speed (EvilDrop_StateBounce1).
 *
 * self: the capsule. Returns flags & 8. Never runs in the corpus. */
#include "xmen2.h"

short EvilDrop_StateFall(EvilDrop *self)
{
	register short y = self->y >> 8;

	if (y >= self->floorY) {
		self->y = (long)self->floorY << 8;
		self->vy = -(self->vy >> 1);
		SET_STATE(self, EvilDrop_StateBounce1);
		return self->flags & 8;
	} else {
		SET_STATE(self, EvilDrop_StateFall);
		return self->flags & 8;
	}
}
