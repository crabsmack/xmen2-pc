/* Gambit_AirThrow - GAMBIT state: the air throw: throw the cards (Gambit_ThrowCards) and wait
 * for the landing or the animation's end (Gambit_StateAirThrowEnd). Already landed: aim on the
 * ground instead (Gambit_GroundAim, now).
 *
 * self: the player. Returns self->flags & 8 after the throw, no value otherwise. */
#include "xmen2.h"

short Gambit_AirThrow(register Gambit *self)
{
	if (!(self->flags & 0x1000)) {
		GOTO_STATE(self, (StateFn)Gambit_GroundAim);
		RETURN_NO_VALUE;
	}
	Gambit_ThrowCards(self);
	SET_STATE(self, (StateFn)Gambit_StateAirThrowEnd);
	return self->flags & 8;
}
