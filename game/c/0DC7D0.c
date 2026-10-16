/* Barrier_StateStart - BARRIER's first state: with no trigger it closes at once
 * (Barrier_Close, then Barrier_StateIdle), else it watches the players (Barrier_StateWatch).
 *
 * self: the barrier. Returns self->flags & 8. */
#include "xmen2.h"

short Barrier_StateStart(register Barrier *self)
{
	if (!self->trigger) {
		Barrier_Close(self);
		SET_STATE(self, Barrier_StateIdle);
		return self->flags & 8;
	}
	SET_STATE(self, Barrier_StateWatch);
	return self->flags & 8;
}
