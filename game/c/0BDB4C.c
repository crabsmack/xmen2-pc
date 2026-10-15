/* Barbarian_StateWatchP2 - BARBARIAN state: go for player 2 if it stands on one of the segments
 * BARBARIAN can reach (Barbarian_StateEngage), else check the idle countdown
 * (Barbarian_StateIdleTimer).
 *
 * self: the object.
 * Returns self->flags & 8. */
#include "xmen2.h"

short Barbarian_StateWatchP2(Barbarian *self)
{
	if (Barbarian_TargetPlayer(self, &g_Player2)) {
		SET_STATE(self, Barbarian_StateEngage);
		return self->flags & 8;
	}
	SET_STATE(self, Barbarian_StateIdleTimer);
	return self->flags & 8;
}
