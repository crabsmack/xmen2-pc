/* Tusk_AtkCharge - TUSK attack step (the target is on its ledge): enter the enemy body hit list
 * for 19 frames and charge after 21 (Tusk_StateChargeStart).
 *
 * self: TUSK.
 * Returns 0. */
#include "xmen2.h"

short Tusk_AtkCharge(Tusk *self)
{
	self->bodyNode = HitList_Add(self, &g_HitListEnemyBody);
	self->hittable = 19;
	self->state = Tusk_StateChargeStart;
	self->timer = 21;
	return 0;
}
