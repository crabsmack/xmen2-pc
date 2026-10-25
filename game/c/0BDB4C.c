/* Barbarian_StateWatchP2 - BARBARIAN state: go for player 2 if it stands on one of the segments
 * BARBARIAN can reach (Barbarian_StateEngage), else check the idle countdown
 * (Barbarian_StateIdleTimer).
 *
 * self: the object.
 * Returns self->flags & 8. */
#include "xmen2.h"

short Barbarian_StateWatchP2(Barbarian *self)
{
	if (Barbarian_TargetPlayer(self, &g_Player2)
#if defined(XMEN2_NATIVE)	/* phase 7: then players 3-4 in play (R11) */
	    || P34_Any([&](XPlayer *q) { return Barbarian_TargetPlayer(self, q); })
#endif
	    ) {
		SET_STATE(self, Barbarian_StateEngage);
		return self->flags & 8;
	}
	SET_STATE(self, Barbarian_StateIdleTimer);
	return self->flags & 8;
}
