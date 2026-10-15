/* Barbarian_StateWatchP1 - BARBARIAN state: go for player 1 if it stands on one of the segments
 * BARBARIAN can reach (Barbarian_StateEngage), else look at player 2 (Barbarian_StateWatchP2).
 *
 * self: the object.
 * Returns self->flags & 8. */
#include "xmen2.h"

short Barbarian_StateWatchP1(Barbarian *self)
{
	if (Barbarian_TargetPlayer(self, &g_Player1)) {
		SET_STATE(self, Barbarian_StateEngage);
		return self->flags & 8;
	}
	SET_STATE(self, Barbarian_StateWatchP2);
	return self->flags & 8;
}
