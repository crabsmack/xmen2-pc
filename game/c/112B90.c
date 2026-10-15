/* Deathbird_StateHoverPause - DEATHBIRD state after a swoop: wait for countdown, then circle
 * (SetMoveMode 2) and throw a burst (Deathbird_StateThrowBurst).
 *
 * self: the boss. Returns self->flags & 8. */
#include "xmen2.h"

short Deathbird_StateHoverPause(Deathbird *self)
{
	if (self->countdown <= 0) {
		(*self->cls->method[27])(self, 2);	/* SetMoveMode */
		SET_STATE(self, (StateFn)Deathbird_StateThrowBurst);
		return self->flags & 8;
	}
	SET_STATE(self, (StateFn)Deathbird_StateHoverPause);
	return self->flags & 8;
}
