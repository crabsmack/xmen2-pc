/* BroodBoss_SpawnDebris - BROODBOSS: an explosion of debris (type 6, the boss debris pool) at its
 * position.
 *
 * self: the boss. */
#include "xmen2.h"

#define g_BroodBossDebris	(*(PTR(DebrisPool) *)g_SceneVar3B14)	/* scene variable: the boss's explosion debris pool */

void BroodBoss_SpawnDebris(BroodBoss *self)
{
	register short x = self->x >> 8;
	register short y = self->y >> 8;

	Debris_Spawn((short)x, (short)y, g_BroodBossDebris, 6);
}
