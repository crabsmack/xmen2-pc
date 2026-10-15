/* Cyclops_GroundSpecialUp - CYCLOPS Up+A on the ground (class slot +$E6): the same as A
 * (Cyclops_GroundAim, now; the aim state reads Up itself).
 *
 * self: the player. */
#include "xmen2.h"

void Cyclops_GroundSpecialUp(Cyclops *self)
{
	GOTO_STATE(self, (StateFn)Cyclops_GroundAim);
}
