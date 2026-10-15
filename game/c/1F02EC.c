/* XPlayer_GroundSpecialUp - Up+A on the ground (class slot +$E6): the neutral ground special
 * (slot +$DE), now.
 *
 * self: the player. */
#include "xmen2.h"

void XPlayer_GroundSpecialUp(XPlayer *self)
{
	GOTO_STATE(self, (StateFn)self->cls->method[54]);	/* GroundSpecial */
}
