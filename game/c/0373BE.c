/* MagnetoBoss_StateFlyToSmash2 - MAGNETOBOSS: once past x $490, smash the floor segment in
 * column $25 (MagnetoBoss_SmashSegmentStaggered), and 5 frames later the one in column $26
 * (MagnetoBoss_StateSmash2b).
 *
 * self: the boss.
 * Returns self->flags & 8 when the state changes, nothing otherwise. */
#include "xmen2.h"

short MagnetoBoss_StateFlyToSmash2(register MagnetoBoss *self)
{
	if (self->x >> 8 > 0x490) {
		MagnetoBoss_SmashSegmentStaggered(self, 0x25);
		SET_STATE_WAIT(self, MagnetoBoss_StateSmash2b, 5);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
