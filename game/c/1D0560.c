/* Gambit_StateAirKick - GAMBIT state during the air kick: on landing, or once actionTimer (12 frames)
 * runs out, the kick is used (playerFlags 2) and the control state (slot +$A6) runs, now.
 *
 * self: the player. */
#include "xmen2.h"

void Gambit_StateAirKick(register Gambit *self)
{
	if (!(self->flags & 0x1000) || self->actionTimer <= 0) {
		self->playerFlags |= 2;
		GOTO_STATE(self, (StateFn)self->cls->method[40]);	/* StateControl */
	}
}
