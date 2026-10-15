/* XPlayer_AirSpecial - the base special in the air (A, class slot +$C6): only steer in the air,
 * then back to the control state (StateControl, slot +$A6) from the next update.
 *
 * self: the player.
 * Returns self->flags & 8, as every state. The characters override it. */
#include "xmen2.h"

short XPlayer_AirSpecial(XPlayer *self)
{
	(*self->cls->method[33])(self);				/* AirSteer */
	SET_STATE(self, (StateFn)self->cls->method[40]);	/* StateControl */
	return self->flags & 8;
}
