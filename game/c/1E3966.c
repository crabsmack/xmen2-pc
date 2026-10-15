/* Cyclops_AirCharge - CYCLOPS the air specials (A, Up+A, Down+A in the air): hold A to aim
 * (Cyclops_StateAirCharge, now).
 *
 * self: the player. */
#include "xmen2.h"

void Cyclops_AirCharge(Cyclops *self)
{
	GOTO_STATE(self, (StateFn)Cyclops_StateAirCharge);
}
