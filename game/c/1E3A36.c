/* Cyclops_GroundSpecialDown - CYCLOPS Down+A on the ground (class slot +$E2): crouch and aim the
 * beam low (Cyclops_CrouchAim, now).
 *
 * self: the player. */
#include "xmen2.h"

void Cyclops_GroundSpecialDown(Cyclops *self)
{
	GOTO_STATE(self, (StateFn)Cyclops_CrouchAim);
}
