/* Tusk_AtkVulnerable - TUSK attack step: enter the enemy body hit list for 45 frames (Tusk_Update
 * counts them down) and choose again after 50 (Tusk_StateVulnerableWait).
 *
 * self: TUSK.
 * Returns 0. */
#include "xmen2.h"

short Tusk_AtkVulnerable(Tusk *self)
{
	self->bodyNode = HitList_Add(self, &g_HitListEnemyBody);
	self->hittable = 45;
	self->countdown = 50;
	self->state = Tusk_StateVulnerableWait;
	return 0;
}
