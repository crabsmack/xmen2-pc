/* BroodBoss_StateDeathSink - BROODBOSS state: lower the boss offset x by $A0 a frame (1/256 px:
 * 0.625 px; $140 in two-player step mode) until it is $1A8 px or less (compared >> 8); then
 * BroodBoss_StateExplode.
 *
 * self: the boss. Returns flags & 8. */
#include "xmen2.h"

short BroodBoss_StateDeathSink(BroodBoss *self)
{
	register short step = 0xA0;

	if (g_TwoPlayerStep)
		step <<= 1;
	if ((g_BroodBossOffsetX >> 8) > 0x1A8) {
		g_BroodBossOffsetX -= step;
		SET_STATE(self, BroodBoss_StateDeathSink);
		return self->flags & 8;
	}
	SET_STATE(self, BroodBoss_StateExplode);
	return self->flags & 8;
}
