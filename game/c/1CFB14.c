/* Gambit_StateThrow - GAMBIT state once a throw is aimed (A released): in the air the air throw
 * (Gambit_AirThrow, now); on the ground the ground throw in 5 frames (Gambit_GroundThrow).
 *
 * self: the player. Returns self->flags & 8, or what Gambit_AirThrow returned. */
#include "xmen2.h"

short Gambit_StateThrow(register Gambit *self)
{
	if (self->flags & 0x1000) {
		GOTO_STATE(self, Gambit_AirThrow);
		RETURN_NO_VALUE;
	}
	SET_STATE_WAIT(self, (StateFn)Gambit_GroundThrow, 5);
	return self->flags & 8;
}
