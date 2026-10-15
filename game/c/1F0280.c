/* XPlayer_AirSpecialUp - Up+A in the air (class slot +$CA): the neutral air special (slot +$C6),
 * now.
 *
 * self: the player. */
#include "xmen2.h"

void XPlayer_AirSpecialUp(XPlayer *self)
{
	GOTO_STATE(self, (StateFn)self->cls->method[48]);	/* AirSpecial */
}
