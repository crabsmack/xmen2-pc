/* Cyclops_GroundSpecial - CYCLOPS A on the ground (class slot +$DE): stop and aim the beam
 * (Cyclops_GroundAim, now).
 *
 * self: the player. */
#include "xmen2.h"

void Cyclops_GroundSpecial(Cyclops *self)
{
	GOTO_STATE(self, (StateFn)Cyclops_GroundAim);
}
