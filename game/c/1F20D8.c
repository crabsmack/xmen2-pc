/* XPlayer_StateFrozen - the frozen state (XPlayer_FreezeStanding): keeps the delay at 255, so no
 * state runs again.
 *
 * self: the player. */
#include "xmen2.h"

void XPlayer_StateFrozen(XPlayer *self)
{
	self->timer = 0xFF;
}
