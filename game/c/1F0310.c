/* XPlayer_GroundSpecialDown - Down+A on the ground (class slot +$E2): the neutral ground special
 * (slot +$DE), now.
 *
 * self: the player. */
#include "xmen2.h"

void XPlayer_GroundSpecialDown(XPlayer *self)
{
	GOTO_STATE(self, (StateFn)self->cls->method[54]);	/* GroundSpecial */
}
