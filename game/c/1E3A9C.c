/* Cyclops_AirSpecialDown - CYCLOPS Down+A in the air (class slot +$CE): the same as A in the air
 * (Cyclops_AirCharge, now).
 *
 * self: the player. */
#include "xmen2.h"

void Cyclops_AirSpecialDown(Cyclops *self)
{
	GOTO_STATE(self, (StateFn)Cyclops_AirCharge);
}
