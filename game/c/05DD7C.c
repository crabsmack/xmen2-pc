/* Tuskette_Move - TUSKETTE Move (class slot +$4E; replaces the parent's): n times, add the speed
 * to the position and the acceleration to the speed, with no speed limit and no floor or wall
 * checks.
 *
 * self: the object.
 * n:    steps (1, or 2 in two-player mode); 0 would loop 65536 times. */
#include "xmen2.h"

void Tuskette_Move(Tuskette *self, short n)
{
	do {
		self->x += self->vx;
		self->y += self->vy;
		self->vx += self->ax;
		self->vy += self->ay;
	} while (--n);
}
