/* MagnetoBoss_StateFlyToSmash1 - MAGNETOBOSS: once past x $2B2, smash the floor segment in
 * column $16 (MagnetoBoss_SmashSegment) and go on to the next chase (MagnetoBoss_Zone2Setup).
 *
 * self: the boss.
 * Returns self->flags & 8 when the state changes, nothing otherwise. */
#include "xmen2.h"

short MagnetoBoss_StateFlyToSmash1(register MagnetoBoss *self)
{
	if (self->x >> 8 > 0x2B2) {
		MagnetoBoss_SmashSegment(self, 0x16);
		SET_STATE(self, MagnetoBoss_Zone2Setup);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
