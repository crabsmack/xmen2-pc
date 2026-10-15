/* Gambit_AirCharge - hold A in the air to charge a throw (Gambit_StateAirCharge, now).
 *
 * self: the player (the three air special slots). */
#include "xmen2.h"

void Gambit_AirCharge(register Gambit *self)
{
	GOTO_STATE(self, (StateFn)Gambit_StateAirCharge);
}
