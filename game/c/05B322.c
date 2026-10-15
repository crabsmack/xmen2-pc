/* Boulder_SpawnDebris - throw rock debris from (x, y): 2 small pieces, then 3 big ones (as many
 * as the pool has records left), each at a random offset (x -255..255, y -255..127, in 1/256
 * px), with a random speed (small: vx -$7FF..$7FF, big -$3FF..$3FF; vy
 * -$1FF..$FF), drag against vx, gravity $20, a random animation start, bouncing at floorY - 16
 * (small) or - 24 (big) on screen (Boulder_UpdateDebris).
 *
 * x, y: pixels.  floorY: screen y of the floor line.
 * MPW draws the left Rand_Next() of each difference first; C++ leaves the order open, so the PC
 * version draws it in a statement of its own (the #ifdefs). */
#include "xmen2.h"

void Boulder_SpawnDebris(register short x, register short y, register short floorY)
{
	register BoulderDebris *d;
	register short i;
#ifndef XMEN2_MPW
	long r;		/* the left Rand_Next() of a - b, drawn first (MPW C's order) */
#endif

	for (i = 0; i < 2; i++) {
		if (!(d = Boulder_AllocDebris()))
			break;
#ifdef XMEN2_MPW
		d->x = (x << 8) + (((unsigned short)Rand_Next() & 0xFF)
			- ((unsigned short)Rand_Next() & 0xFF));
		d->y = (y << 8) + (((unsigned short)Rand_Next() & 0x7F)
			- ((unsigned short)Rand_Next() & 0xFF));
		d->vx = ((unsigned short)Rand_Next() & 0x7FF) - ((unsigned short)Rand_Next() & 0x7FF);
		d->vy = ((unsigned short)Rand_Next() & 0xFF) - ((unsigned short)Rand_Next() & 0x1FF);
#else
		r = (unsigned short)Rand_Next() & 0xFF;
		d->x = (x << 8) + (r - ((unsigned short)Rand_Next() & 0xFF));
		r = (unsigned short)Rand_Next() & 0x7F;
		d->y = (y << 8) + (r - ((unsigned short)Rand_Next() & 0xFF));
		r = (unsigned short)Rand_Next() & 0x7FF;
		d->vx = (short)(r - ((unsigned short)Rand_Next() & 0x7FF));
		r = (unsigned short)Rand_Next() & 0xFF;
		d->vy = (short)(r - ((unsigned short)Rand_Next() & 0x1FF));
#endif
		if (d->vx > 0)
			d->ax = -1;
		else
			d->ax = 1;
		d->ay = 0x20;
		d->frame = (unsigned short)Rand_Next() & 7;
		d->big = 0;
		d->floorY = floorY - 0x10;
	}
	for (i = 0; i < 3; i++) {
		if (!(d = Boulder_AllocDebris()))
			break;
#ifdef XMEN2_MPW
		d->x = (x << 8) + (((unsigned short)Rand_Next() & 0xFF)
			- ((unsigned short)Rand_Next() & 0xFF));
		d->y = (y << 8) + (((unsigned short)Rand_Next() & 0x7F)
			- ((unsigned short)Rand_Next() & 0xFF));
		d->vx = ((unsigned short)Rand_Next() & 0x3FF) - ((unsigned short)Rand_Next() & 0x3FF);
		d->vy = ((unsigned short)Rand_Next() & 0xFF) - ((unsigned short)Rand_Next() & 0x1FF);
#else
		r = (unsigned short)Rand_Next() & 0xFF;
		d->x = (x << 8) + (r - ((unsigned short)Rand_Next() & 0xFF));
		r = (unsigned short)Rand_Next() & 0x7F;
		d->y = (y << 8) + (r - ((unsigned short)Rand_Next() & 0xFF));
		r = (unsigned short)Rand_Next() & 0x3FF;
		d->vx = (short)(r - ((unsigned short)Rand_Next() & 0x3FF));
		r = (unsigned short)Rand_Next() & 0xFF;
		d->vy = (short)(r - ((unsigned short)Rand_Next() & 0x1FF));
#endif
		if (d->vx > 0)
			d->ax = -1;
		else
			d->ax = 1;
		d->ay = 0x20;
		d->frame = (unsigned short)Rand_Next() & 3;
		d->big = 1;
		d->floorY = floorY - 0x18;
	}
}
