/* Hornet_MoveBounce - HORNET motion (+$94) of a DEATHBIRD hornet's bounce pattern, also the
 * flight part of the dive patterns: fly by its velocity and hover on the circle path, bouncing
 * off the arena box; when its lifetime is over (moveFlags bit 2), hover and leave
 * (Hornet_MoveWaitThenFlyOff, at once: countdown 0).
 *
 * self: the hornet.
 * One step per frame, two in two-player step mode. moveFlags bit 0: x bounces between $3C and
 * $1A4 px, bit 1: y between $104 and $172 px (put back on the edge, that speed negated). */
#include "xmen2.h"

void Hornet_MoveBounce(Hornet *self)
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
	if (self->moveFlags & 1) {
		n = self->x >> 8;
		if (n > 0x1A4) {
			self->x = 0x1A4 << 8;
			self->vx = -self->vx;
		} else if (n < 0x3C) {
			self->x = 0x3C << 8;
			self->vx = -self->vx;
		}
	}
	if (self->moveFlags & 2) {
		n = self->y >> 8;
		if (n > 0x172) {
			self->y = 0x172 << 8;
			self->vy = -self->vy;
		} else if (n < 0x104) {
			self->y = 0x104 << 8;
			self->vy = -self->vy;
		}
	}
	if (self->moveFlags & 4) {
		self->countdown = 0;
		self->move = (HornetMoveFn)Hornet_MoveWaitThenFlyOff;
	}
}
