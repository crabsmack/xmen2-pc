/* BroodBoss_StateFinish - BROODBOSS state, the end: a new BROODCLAW off the right edge of the view
 * (placement x camera + $1E0, y $660), spawning on again (g_SpawnEnabled), and the boss kills
 * itself.
 *
 * self: the boss. */
#include "xmen2.h"

void BroodBoss_StateFinish(BroodBoss *self)
{
	Placement pl;

	Mem_Set(&pl, 0, sizeof(pl));
	pl.x = g_CameraX + 0x1E0;
	pl.y = 0x660;
	BroodClaw_Spawn(&pl, 1);
	g_SpawnEnabled = 1;
	(*self->cls->method[19])(self);			/* +$52 Kill */
}
