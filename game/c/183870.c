/* Beast_StateRollRecover - after a roll that ended in the air (Beast_StateRollEnd): wait for the
 * landing or for countdown to run out, then the control state (slot +$A6), now.
 *
 * self: the player.
 * Returns self->flags & 8 while it waits; nothing after. */
#include "xmen2.h"

short Beast_StateRollRecover(register Beast *self)
{
	if (self->flags & 0x1000 && self->countdown > 0) {
		SET_STATE(self, (StateFn)Beast_StateRollRecover);
		return self->flags & 8;
	}
	GOTO_STATE(self, (StateFn)self->cls->method[40]);	/* StateControl */
	NO_RETURN_VALUE
}
