/* MagnetoBoss_StateSmash2b - MAGNETOBOSS: smash the floor segment in column $26 too
 * (MagnetoBoss_SmashSegmentStaggered), then the next chase (MagnetoBoss_Zone4Setup).
 *
 * self: the boss.
 * Returns self->flags & 8. */
#include "xmen2.h"

short MagnetoBoss_StateSmash2b(register MagnetoBoss *self)
{
	MagnetoBoss_SmashSegmentStaggered(self, 0x26);
	SET_STATE(self, MagnetoBoss_Zone4Setup);
	return self->flags & 8;
}
