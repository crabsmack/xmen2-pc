/* Gambit_GroundSpecialDown - GAMBIT GroundSpecialDown (class slot +$E2): Down+A on the ground
 * aims cards crouched (Gambit_CrouchAim, now).
 *
 * self: the player. */
#include "xmen2.h"

void Gambit_GroundSpecialDown(register Gambit *self)
{
	GOTO_STATE(self, (StateFn)Gambit_CrouchAim);
}
