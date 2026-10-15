/* Cyclops_AirSpecial - CYCLOPS A in the air (class slot +$C6): hold to aim, release to fire
 * (Cyclops_AirCharge, now).
 *
 * self: the player. */
#include "xmen2.h"

void Cyclops_AirSpecial(Cyclops *self)
{
	GOTO_STATE(self, (StateFn)Cyclops_AirCharge);
}
