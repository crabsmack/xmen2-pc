/* MagnetoBoss_SpawnRubble - break a wall or floor: set a map column's rows to a block and throw
 * rubble from each of them (sound $34).
 *
 * col:    map column (blocks).  row0, row1: first and last row.
 * perRow: pieces per row.  mirror: throw them left (speed and x acceleration negated).
 * floorY: y (pixels) they bounce off, less 8 or 16 by piece size.  tile: the new block.
 * The pieces come from the class's pool (g_ClassData[g_MagnetoRubbleClass], 32 MagnetoRubble)
 * in order, and a timer (MagnetoBoss_RubbleTimer) moves them; without a free timer nothing
 * happens. Each piece starts at its row's block centre + (-31..15, -31..15) px, with random
 * vx $101-$5FF (right), vy -$29F..-$1E1, ax 0-3, ay $20-$2F; pieces 0, 1, 2 (mod 4, and 3 like
 * 0) are small, middle and large (Tbl_MagnetoBossRubbleSprites entries 0/3/6, 1/2/4 frames per
 * animation frame); the frame starts at 0 or 1 by piece. Every call starts at the pool's first
 * piece; rows stop when the pool runs out, and the pieces after the last one thrown (also any
 * still flying from an earlier call) are deactivated. Each call starts a timer of its own.
 * The pool pointer is also kept in a local that only compiled-out code read (the if (0)).
 * MPW C draws the left Rand_Next() of each difference first; C++ leaves that order open, so
 * the PC version draws it in a statement of its own (the #ifdefs). */
#include "xmen2.h"

void MagnetoBoss_SpawnRubble(short col, short row0, short row1, short perRow, char mirror,
	short floorY, short tile)
{
	unsigned char frameBase, frameDelay;
	short vy;
	unsigned char ay;
	MagnetoRubble *pool;
	short row;
	long fx, fy;
	Timer *timer;
	register MagnetoRubble *p;
	register MagnetoRubble *end;
	register long j;
	register unsigned char drop;
	register short vx;
	register unsigned char ax;
#ifndef XMEN2_MPW
	long r;		/* the left Rand_Next() of a - b, drawn first (MPW C's order; C++'s is open) */
#endif

	pool = p = g_ClassData[g_MagnetoRubbleClass];
	end = &p[32];
	row = row0;
	fx = ((col << 5) + 16) << 8;
	fy = ((row << 5) + 16) << 8;
	if (timer = Timer_Create((TimerFn)MagnetoBoss_RubbleTimer, 0, p, 1)) {
		Timer_Start(timer);
		for (; p < end && row <= row1; row++, fy += 0x2000) {
			Map_SetBlockFG(col, row, tile, 0);
			for (j = 0; j < perRow && p < end; j++, p++) {
#ifdef XMEN2_MPW
				vx = ((unsigned short)Rand_Next() & 0x3FF) + 0x200
					- ((unsigned short)Rand_Next() & 0xFF);
#else
				r = ((unsigned short)Rand_Next() & 0x3FF) + 0x200;
				vx = r - ((unsigned short)Rand_Next() & 0xFF);
#endif
				ax = (unsigned short)Rand_Next() & 3;
				if (mirror) {
					vx = -vx;
					ax = -ax;
				}
#ifdef XMEN2_MPW
				p->x = fx + ((((unsigned short)Rand_Next() & 0xF)
					- ((unsigned short)Rand_Next() & 0x1F)) << 8);
#else
				r = (unsigned short)Rand_Next() & 0xF;
				p->x = fx + ((r - ((unsigned short)Rand_Next() & 0x1F)) << 8);
#endif
				p->vx = vx;
				p->ax = ax;
#ifdef XMEN2_MPW
				vy = ((unsigned short)Rand_Next() & 0x3F) - ((unsigned short)Rand_Next() & 0x7F)
					+ -0x220;
#else
				r = (unsigned short)Rand_Next() & 0x3F;
				vy = r - ((unsigned short)Rand_Next() & 0x7F) + -0x220;
#endif
				ay = ((unsigned short)Rand_Next() & 0xF) + 0x20;
#ifdef XMEN2_MPW
				p->y = fy + ((((unsigned short)Rand_Next() & 0xF)
					- ((unsigned short)Rand_Next() & 0x1F)) << 8);
#else
				r = (unsigned short)Rand_Next() & 0xF;
				p->y = fy + ((r - ((unsigned short)Rand_Next() & 0x1F)) << 8);
#endif
				p->vy = vy;
				p->ay = ay;
				switch (j & 3) {
				default:
				case 0:
					frameBase = 0;
					frameDelay = 1;
					drop = 8;
					break;
				case 1:
					frameBase = 3;
					frameDelay = 2;
					drop = 0x10;
					break;
				case 2:
					frameBase = 6;
					frameDelay = 4;
					drop = 0x10;
					break;
				}
				p->frameBase = frameBase;
				p->frameTimer = frameDelay;
				p->frameDelay = frameDelay;
				p->floorY = floorY - drop;
				p->frame = j & 1;
				p->active = 1;
			}
		}
		for (; p < end; p++)
			p->active = 0;
		Snd_Stop(0x34);
		Snd_Play(0x34);
	}
	if (0)
		p = pool;
}
