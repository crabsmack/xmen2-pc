/* DropBomb_StateFall - DROPBOMB state: move by its speed and accelerate (twice in the two-player
 * step mode); once its y reaches floorY it stops and explodes (DropBomb_StateExplode next frame).
 *
 * self: the bomb.
 * Returns flags & 8 on landing; nothing while falling (state functions' results are not read
 * there). */
#include "xmen2.h"

short DropBomb_StateFall(DropBomb *self)
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
	if ((self->y >> 8) >= self->floorY) {
		self->vy = 0;
		self->ay = 0;
		SET_STATE(self, (StateFn)DropBomb_StateExplode);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
