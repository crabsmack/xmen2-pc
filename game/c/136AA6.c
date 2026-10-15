/* Hornet_MoveWaitThenRise - HORNET motion (+$94) of pattern 3's loop: hover on the circle path at
 * the bottom until countdown runs out, then rise toward the arena's middle (speed $380 both ways,
 * to the left when right of x $F0) with Hornet_MoveRiseThenWait.
 *
 * self: the hornet.
 * One hover step per frame, two in two-player step mode. */
#include "xmen2.h"

void Hornet_MoveWaitThenRise(Hornet *self)
{
	short n = g_TwoPlayerStep ? 2 : 1;

	do {
		if (self->path)
			Hornet_StepCirclePath(self);
	} while (--n);
	if (!self->countdown) {
		short px = self->x >> 8;
		short vx = 0x380;
		short vy = -0x380;

		if (px > 0xF0)
			vx = -vx;
		self->vx = vx;
		self->vy = vy;
		self->move = (HornetMoveFn)Hornet_MoveRiseThenWait;
	}
}
