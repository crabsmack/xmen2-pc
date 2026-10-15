/* Cyclops_AirSpecialUp - CYCLOPS Up+A in the air (class slot +$CA): the same as A in the air
 * (Cyclops_AirCharge, now).
 *
 * self: the player. */
#include "xmen2.h"

void Cyclops_AirSpecialUp(Cyclops *self)
{
	GOTO_STATE(self, (StateFn)Cyclops_AirCharge);
}
