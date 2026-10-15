/* Gambit_GroundSpecialUp - GAMBIT GroundSpecialUp (class slot +$E6): Up+A on the ground is the
 * same as A (Gambit_GroundAim, now; the aim state reads Up).
 *
 * self: the player. */
#include "xmen2.h"

void Gambit_GroundSpecialUp(register Gambit *self)
{
	GOTO_STATE(self, (StateFn)Gambit_GroundAim);
}
