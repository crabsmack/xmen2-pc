/* CoreElevator_MoveUp - COREELEVATOR state while going up: past the upper stop, put it there,
 * mark it stopped (flags6 bit $4000) and go to CoreElevator_WaitAtTop.
 *
 * self: the lift. Returns flags & 8 when it stopped; nothing (whatever d0 held) while moving. */
#include "xmen2.h"

short CoreElevator_MoveUp(register CoreElevator *self)
{
	register long stop = (long)self->top << 8;

	if (stop > self->y) {
		self->y = stop;
		self->flags6 |= 0x4000;
		SET_STATE(self, CoreElevator_WaitAtTop);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
