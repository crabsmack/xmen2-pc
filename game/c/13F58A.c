/* Debris_Spawn - throw n debris pieces of a pool from a point, each with random offsets and
 * velocities (Rand_Next), until the pool is full.
 *
 * x, y: the point (pixels).  pool: the debris pool.  n: pieces.
 * Each piece: position x, y +/- up to $3FF (8 fraction bits, a difference of two draws), vx
 * +/- $7FF, vy +/- $3FF, dirX 2 with vx's sign, gravity $20, frame 0-3, bounce 0-1, frame delay
 * 4-7, spin 0-255.
 * MPW C draws the left Rand_Next() of each difference first; C++ leaves that order open, so
 * the PC build draws it into a variable first. */
#include "xmen2.h"

#define RND(m)	((unsigned short)Rand_Next() & (m))

void Debris_Spawn(register short x, register short y, register DebrisPool *pool, register short n)
{
	DebrisPiece *pieces = (DebrisPiece *)&pool[1];
	register short i;
	register DebrisPiece *p;
#ifndef XMEN2_MPW
	long r;		/* the left draw of a difference, drawn first (MPW C's order) */
#endif

	for (i = 0; i < n; i++) {
		p = pool->used < pool->max ? &pieces[pool->used++] : 0;
		if (!p)
			return;
#ifdef XMEN2_MPW
		p->x = ((long)x << 8) + (RND(0x3FF) - RND(0x3FF));
		p->y = ((long)y << 8) + (RND(0x3FF) - RND(0x3FF));
		p->vx = RND(0x7FF) - RND(0x7FF);
		p->vy = RND(0x3FF) - RND(0x3FF);
#else
		r = RND(0x3FF);
		p->x = ((long)x << 8) + (r - RND(0x3FF));
		r = RND(0x3FF);
		p->y = ((long)y << 8) + (r - RND(0x3FF));
		r = RND(0x7FF);
		p->vx = (short)(r - RND(0x7FF));
		r = RND(0x3FF);
		p->vy = (short)(r - RND(0x3FF));
#endif
		if (p->vx > 0)
			p->dirX = 2;
		else
			p->dirX = -2;
		p->gravity = 0x20;
		p->frame = RND(3);
		p->bounce = RND(1);
		p->frameDelay = RND(3) + 4;
		p->spin = RND(0xFF);
	}
}
