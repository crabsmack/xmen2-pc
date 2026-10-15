/* EvilMaker_StateIdle - EVILMAKER AI script step: stay in this state (it never advances the
 * script).
 *
 * self: EVILMAKER.
 * Returns self->flags & 8. Never runs in the corpus. */
#include "xmen2.h"

short EvilMaker_StateIdle(EvilMaker *self)
{
	SET_STATE(self, EvilMaker_StateIdle);
	return self->flags & 8;
}
