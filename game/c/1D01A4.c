/* Gambit_GroundSpecial - GAMBIT GroundSpecial (class slot +$DE): A on the ground aims cards
 * standing (Gambit_GroundAim, now).
 *
 * self: the player. */
#include "xmen2.h"

void Gambit_GroundSpecial(register Gambit *self)
{
	GOTO_STATE(self, (StateFn)Gambit_GroundAim);
}
