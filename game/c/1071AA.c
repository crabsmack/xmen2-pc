/* Platform1_StateShuttle - PLATFORM1 state in mode 0 (shuttle): stopped at a path end (flags bit
 * $100) with the countdown over, it goes back the other way (speed = -savedSpeed).
 *
 * self: the platform. Returns nothing. */
#include "xmen2.h"

void Platform1_StateShuttle(register Platform1 *self)
{
	if (self->flags & 0x100 && self->countdown <= 0) {
		self->flags &= ~0x100;
		self->vx = -self->savedSpeed;
	}
}
