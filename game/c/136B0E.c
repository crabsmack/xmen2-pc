/* Hornet_MoveDiveRepeat - HORNET motion (+$94) of pattern 3 (Hornet_StatePickPattern): the first
 * dive (Hornet_MoveBounce) until y reaches $172 px, then hover 10 frames and start the rise /
 * dive loop (Hornet_MoveWaitThenRise).
 *
 * self: the hornet.
 * The same body as Hornet_MoveDiveAgain. */
#include "xmen2.h"

void Hornet_MoveDiveRepeat(Hornet *self)
{
	short py;

	Hornet_MoveBounce(self);
	py = self->y >> 8;
	if (py >= 0x172) {
		self->countdown = 10;
		self->move = (HornetMoveFn)Hornet_MoveWaitThenRise;
	}
}
