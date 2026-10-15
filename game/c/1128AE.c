/* Deathbird_StateStart - DEATHBIRD's first state (Deathbird_Init): waiting off the left side
 * (moveKind 1), wobble rates 4 and 3, then the hornet waves (Deathbird_StateSendHornets).
 *
 * self: the boss. Returns self->flags & 8. */
#include "xmen2.h"

short Deathbird_StateStart(Deathbird *self)
{
	self->moveKind = 1;
	self->wobbleVx = 4;
	self->wobbleVy = 3;
	SET_STATE(self, (StateFn)Deathbird_StateSendHornets);
	return self->flags & 8;
}
