/* XPlayer_GroundSpecial - the base special on the ground (A, class slot +$DE): none, back to the
 * control state (StateControl, slot +$A6) from the next update.
 *
 * self: the player.
 * Returns self->flags & 8, as every state. The characters override it. */
#include "xmen2.h"

short XPlayer_GroundSpecial(XPlayer *self)
{
	SET_STATE(self, (StateFn)self->cls->method[40]);	/* StateControl */
	return self->flags & 8;
}
