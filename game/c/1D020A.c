/* Gambit_AirSpecial - GAMBIT AirSpecial (class slot +$C6): A in the air charges a throw
 * (Gambit_AirCharge, now).
 *
 * self: the player. */
#include "xmen2.h"

void Gambit_AirSpecial(register Gambit *self)
{
	GOTO_STATE(self, (StateFn)Gambit_AirCharge);
}
