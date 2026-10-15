/* Hornet_MoveVelocity - HORNET motion (+$94) of a DEATHBIRD hornet dropping into the arena
 * (Hornet_SpawnForDeathbird): move by its velocity and accelerate, twice in two-player step mode.
 *
 * self: the hornet. */
#include "xmen2.h"

void Hornet_MoveVelocity(Hornet *self)
{
	self->x += self->vx;
	self->y += self->vy;
	self->vx += self->ax;
	self->vy += self->ay;
	if (g_TwoPlayerStep) {
		self->x += self->vx;
		self->y += self->vy;
		self->vx += self->ax;
		self->vy += self->ay;
	}
}
