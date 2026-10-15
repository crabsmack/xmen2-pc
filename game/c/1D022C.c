/* Gambit_AirSpecialUp - GAMBIT AirSpecialUp (class slot +$CA): Up+A in the air is the same as A
 * (Gambit_AirCharge, now). Never runs in the corpus.
 *
 * self: the player. */
#include "xmen2.h"

void Gambit_AirSpecialUp(register Gambit *self)
{
	GOTO_STATE(self, (StateFn)Gambit_AirCharge);
}
