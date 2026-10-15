/* Tuskette_StateChaseTarget - TUSKETTE of a pair running at its partner: burst on reaching it
 * (within 4 px in x: Tuskette_StateHitTarget); with no partner keep going
 * (Tuskette_StateWanderOut), with a partner out of health give up (Tuskette_StateTargetGone).
 *
 * self: the object. Never runs in the corpus.
 * Returns self->flags & 8 on a change, nothing while running. */
#include "xmen2.h"

short Tuskette_StateChaseTarget(Tuskette *self)
{
	Tuskette *t;

	if (t = self->partner) {
		if (t->health <= 0) {
			SET_STATE(self, Tuskette_StateTargetGone);
			return self->flags & 8;
		} else if (ABS(self->x - t->x) < 0x400) {
			SET_STATE(self, (StateFn)Tuskette_StateHitTarget);
			return self->flags & 8;
		}
	} else {
		SET_STATE(self, Tuskette_StateWanderOut);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
