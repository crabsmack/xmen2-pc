/* Hornet_MoveWaitThenFlyOff - HORNET motion (+$94): hover on the circle path until countdown
 * runs out, then fly off up and forward (speed $500 both ways, x by its facing) with
 * Hornet_MoveFlyOff.
 *
 * self: the hornet.
 * One hover step per frame, two in two-player step mode. */
#include "xmen2.h"

void Hornet_MoveWaitThenFlyOff(Hornet *self)
{
	short n = g_TwoPlayerStep ? 2 : 1;

	do {
		if (self->path)
			Hornet_StepCirclePath(self);
	} while (--n);
	if (!self->countdown) {
		short px = self->x >> 8;
		short vx = 0x500;
		short vy = -0x500;

		if (self->spr.flags & 0x10)
			vx = -vx;
		self->vx = vx;
		self->vy = vy;
		self->move = (HornetMoveFn)Hornet_MoveFlyOff;
	}
}
