/* BroodBoss_StateExplode - BROODBOSS state: the body blows up: three debris explosions (type 6, the
 * boss debris pool) at its position and at (-$14, -$18) and (+$14, -$12) from it, sounds $34 and
 * $CF; then BroodBoss_StateVanish in 2 frames.
 *
 * self: the boss. Returns flags & 8. */
#include "xmen2.h"

#define g_BroodBossDebris	(*(PTR(DebrisPool) *)g_SceneVar3B14)	/* scene variable: the boss's explosion debris pool */

short BroodBoss_StateExplode(BroodBoss *self)
{
	register short x = self->x >> 8;
	register short y = self->y >> 8;

	Debris_Spawn((short)x, (short)y, g_BroodBossDebris, 6);
	Debris_Spawn(x + -0x14, y + -0x18, g_BroodBossDebris, 6);
	Debris_Spawn(x + 0x14, y + -0x12, g_BroodBossDebris, 6);
	Snd_Stop(0x34);
	Snd_Play(0x34);
	Snd_Stop(0xCF);
	Snd_Play(0xCF);
	SET_STATE_WAIT(self, BroodBoss_StateVanish, 2);
	return self->flags & 8;
}
