/* Hornet_MoveFlyOff - HORNET motion (+$94) of a leaving hornet: fly by its velocity and hover on
 * the circle path; once it is more than $28 px past the top of the screen, kill it.
 *
 * self: the hornet.
 * One step per frame, two in two-player step mode. The test compares y with g_CameraX, not
 * g_CameraY, as the original does (DEATHBIRD's arena doesn't scroll). */
#include "xmen2.h"

void Hornet_MoveFlyOff(Hornet *self)
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
	if (n < g_CameraX - 0x28)
		(*self->cls->method[19])(self);		/* Kill */
}
