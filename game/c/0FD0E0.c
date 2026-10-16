/* SiberiaB1_SpawnDebris - throw 4 ice shards from (x, y) (as many as the pool has free): each
 * within 4 px of it, with a random velocity (each axis -$1FF..$1FF), gravity $20, a random first
 * frame set (0 or 3), frame, delay and age.
 *
 * x, y: pixels (the middle of a broken ice block: SiberiaB1_IceTileHit).
 * MPW C draws the left Rand_Next() of each difference first; C++ leaves that order open, so
 * the PC build draws it into a variable first. */
#include "xmen2.h"

#define RND(m)	((unsigned short)Rand_Next() & (m))

void SiberiaB1_SpawnDebris(register short x, register short y)
{
	register IceShard *p;
	register short i;
#ifndef XMEN2_MPW
	long r;		/* the left draw of a difference, drawn first (MPW C's order) */
#endif

	for (i = 0; i < 4; i++) {
		if ((p = SiberiaB1_DebrisAlloc()) == 0)
			break;
#ifdef XMEN2_MPW
		p->x = (x << 8) + (RND(0x3FF) - RND(0x3FF));
		p->y = (y << 8) + (RND(0x3FF) - RND(0x3FF));
		p->vx = RND(0x1FF) - RND(0x1FF);
		p->vy = RND(0x1FF) - RND(0x1FF);
#else
		r = RND(0x3FF);
		p->x = (x << 8) + (r - RND(0x3FF));
		r = RND(0x3FF);
		p->y = (y << 8) + (r - RND(0x3FF));
		r = RND(0x1FF);
		p->vx = (short)(r - RND(0x1FF));
		r = RND(0x1FF);
		p->vy = (short)(r - RND(0x1FF));
#endif
		if (p->vx > 0)
			p->ax = 2;
		else
			p->ax = -2;
		p->ay = 0x20;
		p->frameSet = RND(1) ? 0 : 3;
		p->frame = RND(1);
		p->frameDelay = RND(3);
		p->age = RND(0x3F);
	}
}
