/* MagnetoBoss_StateBreakWall - MAGNETOBOSS: once past x $720, break the wall at column $39,
 * rows 5-7, at once (MagnetoBoss_CrackWall) with rubble flying from it, and go on to the
 * final spot (MagnetoBoss_StateFlyToFinal, also the state to resume after a shot).
 *
 * self: the boss.
 * Returns self->flags & 8 when the state changes, nothing otherwise. */
#include "xmen2.h"

short MagnetoBoss_StateBreakWall(register MagnetoBoss *self)
{
	if (self->x >> 8 > 0x720) {
		MagnetoBoss_CrackWall(0x39, 5, 7, 1);
		MagnetoBoss_SpawnRubble(0x39, 5, 7, 8, 0, 0x180, 0x11);
		self->resumeState = MagnetoBoss_StateFlyToFinal;
		SET_STATE(self, MagnetoBoss_StateFlyToFinal);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
