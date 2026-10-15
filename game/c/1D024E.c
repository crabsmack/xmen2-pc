/* Gambit_AirSpecialDown - GAMBIT AirSpecialDown (class slot +$CE): Down+A in the air is the same
 * as A (Gambit_AirCharge, now). Never runs in the corpus.
 *
 * self: the player. */
#include "xmen2.h"

void Gambit_AirSpecialDown(register Gambit *self)
{
	GOTO_STATE(self, (StateFn)Gambit_AirCharge);
}
