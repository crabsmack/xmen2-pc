/* XPlayer_StateClimbNone - the base climbing state (class slot +$AA): none, back to the control
 * state (StateControl, slot +$A6) from the next update.
 *
 * self: the player.
 * Returns self->flags & 8, as every state. The climbing characters (XCLIMBER) override it. */
#include "xmen2.h"

short XPlayer_StateClimbNone(XPlayer *self)
{
	SET_STATE(self, (StateFn)self->cls->method[40]);	/* StateControl */
	return self->flags & 8;
}
