/* Exodus_StateSwoopStart - EXODUS state: start the swoop down to (targetX, y $2D5): vy 200, and
 * the vx that gets it to targetX in about as many steps, doubled; both x and y move
 * (Exodus_StateSwoopToY waits for the height).
 *
 * self: the boss. Returns self->flags & 8.
 * steps = |dy| / |vy| (both 8 fraction bits), vx = dx / steps, then vx += vx. A 0 step count
 * (less than one step to go) would divide by zero. */
#include "xmen2.h"

short Exodus_StateSwoopStart(register Exodus *self)
{
	register long dx, steps, dy;

	self->targetY = 0x2D5;
	self->vx = 200;
	self->vy = 200;
	dx = (self->targetX << 8) - self->x;
	dy = (self->targetY << 8) - self->y;
	steps = ABS(dy) / ABS(self->vy);
	self->vx = (short)(dx / steps);
	self->vx += self->vx << 0;	/* MPW: the zero shift written out gives the ROM's ext.l */
	self->moveFlags = 0;
	self->moveFlags |= 2;
	self->moveFlags |= 1;
	SET_STATE(self, Exodus_StateSwoopToY);
	return self->flags & 8;
}
