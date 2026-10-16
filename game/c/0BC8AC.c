/* SavageBio_ActCrossIfUp - SAVAGEBIGBIO script action: when above the floor (hanging from the
 * ceiling), cross back down (SavageBio_StateCrossSurface next frame); else go on with the
 * script on the next update.
 *
 * self: the boss (a step of its AI script).
 * Returns self->flags & 8 when crossing; nothing in particular otherwise. */
#include "xmen2.h"

short SavageBio_ActCrossIfUp(SavageBigBio *self)
{
	short y = self->y >> 8;

	if (y < self->arenaFloor) {
		SET_STATE(self, SavageBio_StateCrossSurface);
		return self->flags & 8;
	} else {
		AI_NEXT_STEP(self, self->ai, 0);
	}
	NO_RETURN_VALUE
}
