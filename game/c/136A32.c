/* Hornet_MoveRiseThenWait - HORNET motion (+$94) of pattern 3's loop: rise by its velocity,
 * hovering on the circle path, until y is up to $116 px; then hover 20 frames before the next
 * dive (Hornet_MoveWaitThenDive).
 *
 * self: the hornet.
 * One step per frame, two in two-player step mode. */
#include "xmen2.h"

void Hornet_MoveRiseThenWait(Hornet *self)
{
	short n = g_TwoPlayerStep ? 2 : 1;

	do {
		self->x += self->vx;
		self->y += self->vy;
		self->vx += self->ax;
		self->vy += self->ay;
		if (self->path)
			Hornet_StepCirclePath(self);
	} while (--n);
	n = self->y >> 8;
	if (n <= 0x116) {
		self->countdown = 20;
		self->move = (HornetMoveFn)Hornet_MoveWaitThenDive;
	}
}
