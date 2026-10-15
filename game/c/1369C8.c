/* Hornet_MoveWaitThenDive - HORNET motion (+$94) of pattern 3's loop: hover on the circle path at
 * the top until countdown runs out, then dive forward (vx $380 by its facing, vy $300) with
 * Hornet_MoveDiveAgain.
 *
 * self: the hornet.
 * One hover step per frame, two in two-player step mode. */
#include "xmen2.h"

void Hornet_MoveWaitThenDive(Hornet *self)
{
	short n = g_TwoPlayerStep ? 2 : 1;

	do {
		if (self->path)
			Hornet_StepCirclePath(self);
	} while (--n);
	if (!self->countdown) {
		short px = self->x >> 8;
		short vx = 0x380;
		short vy = 0x300;

		if (self->spr.flags & 0x10)
			vx = -vx;
		self->vx = vx;
		self->vy = vy;
		self->move = (HornetMoveFn)Hornet_MoveDiveAgain;
	}
}
