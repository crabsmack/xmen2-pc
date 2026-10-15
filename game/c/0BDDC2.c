/* Barbarian_StateWalkRight - BARBARIAN walking right: stop at walkMaxX, the end of its segment
 * (Barbarian_StateSegmentEnd), or once past targetX (Barbarian_StateReachedTarget).
 *
 * self: the object.
 * Returns self->flags & 8 when it stops, nothing while walking. */
#include "xmen2.h"

short Barbarian_StateWalkRight(Barbarian *self)
{
	if (self->walkMaxX << 8 < self->x) {
		self->x = self->walkMaxX << 8;
		SET_STATE(self, Barbarian_StateSegmentEnd);
		return self->flags & 8;
	}
	if (self->targetX << 8 < self->x) {
		SET_STATE(self, Barbarian_StateReachedTarget);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
