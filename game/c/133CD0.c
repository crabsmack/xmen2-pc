/* DropRocket_StateFly - DROPROCKET state in level flight: nothing (Move carries it), again next
 * update.
 *
 * self: the rocket. Returns flags & 8. */
#include "xmen2.h"

short DropRocket_StateFly(DropRocket *self)
{
	SET_STATE(self, (StateFn)DropRocket_StateFly);
	return self->flags & 8;
}
