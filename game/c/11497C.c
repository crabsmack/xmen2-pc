/* XNpc_StateDeathFlying - XNPC state: the flung body flies (velocity, acceleration; twice a frame
 * in two-player step mode) until it hits a wall (solid map tiles $4000 at the box edge it flies
 * toward, half way down) or falls below the line in py; then it blows up (Explode).
 *
 * self: the object.
 * Returns flags bit 3 while flying; nothing in particular after Explode. */
#include "xmen2.h"

short XNpc_StateDeathFlying(XNpc *self)
{
	if (self->vx > 0 && (char)Tile_TestAt(0, self->boxRight,
		(self->boxTop + self->boxBottom) >> 1, !g_SolidMapIsBg, 0x4000, 1)) {
		(*self->cls->method[30])(self);	/* +$7E Explode */
		RETURN_NO_VALUE;
	}
	if (self->vx < 0 && (char)Tile_TestAt(0, self->boxLeft,
		(self->boxTop + self->boxBottom) >> 1, !g_SolidMapIsBg, 0x4000, 1)) {
		(*self->cls->method[30])(self);	/* +$7E Explode */
		RETURN_NO_VALUE;
	}
	if ((self->y >> 8) > self->py) {
		(*self->cls->method[30])(self);	/* +$7E Explode */
		RETURN_NO_VALUE;
	}
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
	SET_STATE(self, XNpc_StateDeathFlying);
	return self->flags & 8;
}
