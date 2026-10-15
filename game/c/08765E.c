/* BroodBoss_StateRecoil - BROODBOSS state: recoil from the wall: the body and the boss offset x
 * move out 3 px a frame (6 in two-player step mode) until its x offset reaches 10; then
 * BroodBoss_StateRecoilReturn.
 *
 * self: the boss. Returns flags & 8. */
#include "xmen2.h"

short BroodBoss_StateRecoil(BroodBoss *self)
{
	register short step = 3;

	if (g_TwoPlayerStep)
		step <<= 1;
	if (self->offX < 10) {
		self->offX += step;
		g_BroodBossOffsetX += step << 8;
		SET_STATE(self, BroodBoss_StateRecoil);
		return self->flags & 8;
	}
	SET_STATE(self, (StateFn)BroodBoss_StateRecoilReturn);
	return self->flags & 8;
}
