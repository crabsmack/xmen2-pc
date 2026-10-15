/* BroodBoss_StateRecoilReturn - BROODBOSS state: come back from the recoil, 3 px a frame (6 in
 * two-player step mode) on the body's x offset and the boss offset x, until the offset is 0;
 * then the next AI-script step in 10 frames.
 *
 * self: the boss. Returns flags & 8 while it moves, nothing after. */
#include "xmen2.h"

short BroodBoss_StateRecoilReturn(BroodBoss *self)
{
	register short step = 3;

	if (g_TwoPlayerStep)
		step <<= 1;
	if (self->offX > 0) {
		self->offX -= step;
		g_BroodBossOffsetX -= step << 8;
		SET_STATE(self, BroodBoss_StateRecoilReturn);
		return self->flags & 8;
	}
	self->offX = 0;
	AI_NEXT_STEP(self, self->ai, 10);
	NO_RETURN_VALUE
}
