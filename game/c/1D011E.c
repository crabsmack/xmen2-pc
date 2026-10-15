/* Gambit_GroundAim - stop and aim cards standing (Gambit_StateGroundAim, now).
 *
 * self: the player.
 * From the ground special slots, and when an air throw finds him landed (Gambit_AirThrow,
 * Gambit_StateAirThrowBrake). */
#include "xmen2.h"

void Gambit_GroundAim(register Gambit *self)
{
	self->ax = 0;
	self->vx = 0;
	GOTO_STATE(self, Gambit_StateGroundAim);
}
