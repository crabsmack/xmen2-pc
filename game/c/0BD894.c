/* Barbarian_SetupLeap - set up BARBARIAN's leap to (targetX, targetY) if that point is within
 * $96 px on both axes: a jump under gravity 200 whose rise speed grows with the drop (vy =
 * dy / 32 - 3000), the x speed to cover dx in the time up and down again, facing the way it
 * leaps; speed limits x at least |vx|, y 6000; flags bit $1000 on (leaping), flags6 bit $4000
 * off, the animation stopped and frame $12.
 *
 * self: the object.
 * Returns 1 when it leaps, 0 when the point is out of range (nothing changed).
 * Kept bug: the |dx| term of the rise speed is shifted by 32, which MPW C compiles to the sign of
 * |dx| (0 here); the PC build computes that. */
#include "xmen2.h"

char Barbarian_SetupLeap(Barbarian *self)
{
	long dx;
	long dy;
	long vy;
	long frames;

	dx = (self->targetX << 8) - self->x;
	dy = (self->targetY << 8) - self->y;
	if (ABS(dx) > 0x9600)
		return 0;
	if (ABS(dy) > 0x9600)
		return 0;
#ifdef XMEN2_MPW
	vy = (dy >> 5) + -3000 + (ABS(dx) >> 32);
#else
	vy = (dy >> 5) + -3000 + (ABS(dx) < 0 ? -1 : 0);
#endif
	frames = ABS(vy / 200) << 1;
	if (dx < 0)
		self->spr.flags |= 0x10;
	else
		self->spr.flags &= ~0x10;
	self->vx = dx / frames;
	self->ay = 200;
	self->vy = vy;
	if (ABS(self->vx) > self->maxVx)
		self->maxVx = ABS(self->vx);
	self->maxVy = 6000;
	self->flags |= 0x1000;
	self->flags6 &= ~0x4000;
	self->anim = 0;
	(*self->cls->method[20])(self, 0x12);		/* +$56 SetFrame */
	return 1;
}
