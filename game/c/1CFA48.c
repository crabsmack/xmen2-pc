/* Gambit_StateAirThrowEnd - GAMBIT state after an air throw: back to the control state (slot
 * +$A6), now, on landing or once the throw animation has ended.
 *
 * self: the player. */
#include "xmen2.h"

void Gambit_StateAirThrowEnd(register Gambit *self)
{
	if (!(self->flags & 0x1000))
		GOTO_STATE(self, (StateFn)self->cls->method[40]);	/* StateControl */
	else if (!self->anim)
		GOTO_STATE(self, (StateFn)self->cls->method[40]);
}
