/* Hornet_MoveDiveAgain - HORNET motion (+$94) of pattern 3's loop: dive (Hornet_MoveBounce)
 * until y reaches $172 px, then hover 10 frames and rise again (Hornet_MoveWaitThenRise).
 *
 * self: the hornet.
 * The same body as Hornet_MoveDiveRepeat, the loop's first dive. */
#include "xmen2.h"

void Hornet_MoveDiveAgain(Hornet *self)
{
	short py;

	Hornet_MoveBounce(self);
	py = self->y >> 8;
	if (py >= 0x172) {
		self->countdown = 10;
		self->move = (HornetMoveFn)Hornet_MoveWaitThenRise;
	}
}
