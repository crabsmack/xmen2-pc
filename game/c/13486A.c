/* FlyBrood_MoveSine - FLYBROOD mover (+$9E) of the sine fly-by: x moves by its speed, y follows
 * a sine wave around anchorY (sin(angle) >> shift, angle + angleStep per step, kept in 0-360).
 *
 * self:  the object.
 * count: movement steps this frame (at least 1).
 * (config/compiler/match/13486A.c in xmen2disasm: MPW C gives registers to the most-used locals,
 * ties to the later declaration, and lays the rest out on the stack in declaration order; this
 * order reproduces the ROM's.) */
#include "xmen2.h"

void FlyBrood_MoveSine(FlyBrood *self, short count)
{
	short angle, step, shift;
	long y;
	short speed;
	long x, s;

	angle = self->angle;
	step = self->angleStep;
	shift = self->shift;
	x = self->x;
	speed = self->vx;
	do {
		x += speed;
		angle += step;
		if (angle > 360)
			angle -= 360;
		else if (angle < 0)
			angle += 360;
		s = Math_SinDeg(angle) >> shift;
		y = ((long)self->anchorY << 8) + s;
	} while (--count);
	self->x = x;
	self->y = y;
	self->angle = angle;
}
