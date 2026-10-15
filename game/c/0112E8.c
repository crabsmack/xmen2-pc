/* BigAcolyte_StateJoinList - BIGACOLYTE state: become hittable (flags6 bit 1 off, its body linked
 * into g_HitListEnemyBody), then the next step of its AI script.
 *
 * self: the boss. */
#include "xmen2.h"

void BigAcolyte_StateJoinList(register BigAcolyte *self)
{
	self->flags6 &= ~2;
	self->bodyNode = HitList_Add(self, &g_HitListEnemyBody);
	AI_NEXT_STEP(self, self->ai, 0);
}
