/* Apocalypse_StartJitter - APOCALYPSE: the hover wobble near the end of a damped move: a new
 * acceleration per axis, (vx * vx >> 8) / (2 |distance to the target| << 8), or none when the
 * doubled distance is under 2 px; then wobbling (jitterDelay -100).
 *
 * self: the boss.
 * Both axes use the square of the horizontal speed (vy is never used; kept). */
#include "xmen2.h"

void Apocalypse_StartJitter(register Apocalypse *self)
{
	short dx = (self->targetX - (self->x >> 8)) << 1;
	short dy = (self->targetY - (self->y >> 8)) << 1;
	short adx = ABS(dx);
	short ady = ABS(dy);
	short ax, ay;

	if (adx < 2)
		ax = 0;
	else {
		short v2 = self->vx * self->vx >> 8;
		long n = (long)adx << 8;

		ax = v2 / n;
	}
	if (ady < 2)
		ay = 0;
	else {
		short v2 = self->vx * self->vx >> 8;
		long n = (long)ady << 8;

		ay = v2 / n;
	}
	self->ax = ax;
	self->ay = ay;
	self->jitterDelay = -100;
}
