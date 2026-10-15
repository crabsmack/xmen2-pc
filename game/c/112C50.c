/* Deathbird_StateWaitThenLeave - DEATHBIRD state after throwing a burst: when the animation has
 * ended, fly off (SetMoveMode 4, Deathbird_StateFlyOff).
 *
 * self: the boss. Returns self->flags & 8. */
#include "xmen2.h"

short Deathbird_StateWaitThenLeave(Deathbird *self)
{
	if (self->anim) {
		SET_STATE(self, (StateFn)Deathbird_StateWaitThenLeave);
		return self->flags & 8;
	}
	(*self->cls->method[27])(self, 4);	/* SetMoveMode */
	SET_STATE(self, (StateFn)Deathbird_StateFlyOff);
	return self->flags & 8;
}
