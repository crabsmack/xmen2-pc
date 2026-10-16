/* BrainRocket_StateFly - BRAINROCKET state in flight: nothing (Move carries it), again next
 * update.
 *
 * self: the rocket. Returns flags & 8. */
#include "xmen2.h"

short BrainRocket_StateFly(BrainRocket *self)
{
	SET_STATE(self, (StateFn)BrainRocket_StateFly);
	return self->flags & 8;
}
