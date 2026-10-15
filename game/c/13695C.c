/* Hornet_MoveDiveOnce - HORNET motion (+$94) of pattern 2 (Hornet_StatePickPattern): dive
 * (Hornet_MoveBounce, bouncing in y) until y reaches $172 px, then hover 20 frames and rise away
 * (Hornet_MoveWaitThenRiseLeave).
 *
 * self: the hornet. */
#include "xmen2.h"

void Hornet_MoveDiveOnce(Hornet *self)
{
	short py;

	Hornet_MoveBounce(self);
	py = self->y >> 8;
	if (py >= 0x172) {
		self->countdown = 20;
		self->move = (HornetMoveFn)Hornet_MoveWaitThenRiseLeave;
	}
}
