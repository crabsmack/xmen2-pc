/* XPlayer_AirSpecialDown - Down+A in the air (class slot +$CE): the neutral air special
 * (slot +$C6), now.
 *
 * self: the player. */
#include "xmen2.h"

void XPlayer_AirSpecialDown(XPlayer *self)
{
	GOTO_STATE(self, (StateFn)self->cls->method[48]);	/* AirSpecial */
}
