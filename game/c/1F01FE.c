/* XPlayer_DoubleJumpNone - the base double jump (class slot +$B6): none, back to the control
 * state (StateControl, slot +$A6) from the next update.
 *
 * self: the player.
 * Returns self->flags & 8, as every state. */
#include "xmen2.h"

short XPlayer_DoubleJumpNone(XPlayer *self)
{
	SET_STATE(self, (StateFn)self->cls->method[40]);	/* StateControl */
	return self->flags & 8;
}
