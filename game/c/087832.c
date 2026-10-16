/* BroodBoss_StateMakeUnhittable - BROODBOSS state: unhittable (flags6 bit 1, out of the enemy body
 * hit list), next AI-script step on the next update.
 *
 * self: the boss. */
#include "xmen2.h"

void BroodBoss_StateMakeUnhittable(BroodBoss *self)
{
	self->flags6 |= 2;
	List_Unlink(self->bodyNode, &g_HitListEnemyBody);
	self->bodyNode = 0;
	AI_NEXT_STEP(self, self->ai, 0);
}
