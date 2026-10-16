/* GaussBullet_Fire - fire a gauss bullet from (x, y): take the next record of g_GaussPool (none
 * when all 6 are live) and set it flying.
 *
 * x, y:   start (8 fraction bits).
 * owner:  object it sticks to at offset (dx, dy) instead of moving (flags bit 2 is then set); 0
 *         none (the offset is still stored).
 * dx, dy: that offset in pixels.
 * flags:  bit 0 diagonal (speed $800 in x and y, frames from 5), else level (speed $C00 in x);
 *         $800 to the left, $1000 upward (vertical speed negated); bit 1 clear: the in-place
 *         animation frames (+4); bit 3: no sound ($ED otherwise, restarted); the rest goes to the
 *         record's flags with palette 3 ($6000).
 * The flags are masked with $FFFFFFFF first (no effect). */
#include "xmen2.h"

void GaussBullet_Fire(long x, long y, void *owner, short dx, short dy, register long flags)
{
	register GaussBullet *b = g_GaussPool->count < g_GaussPool->max
		? &g_GaussPool->rec[g_GaussPool->count++] : 0;

	if (b) {
		register short vx = 0xC00;
		register short vy = 0;
		register char frame = 0;

		flags &= 0xFFFFFFFF;
		if (flags & 1) {
			vx = vy = 0x800;
			frame = 5;
		}
		if (flags & 0x800)
			vx = -vx;
		if (flags & 0x1000)
			vy = -vy;
		if (!(flags & 2))
			frame += 4;
		b->owner = 0;
		b->offX = dx;
		b->offY = dy;
		if (owner) {
			b->owner = (Actor *)owner;
			flags |= 4;
		}
		b->flags = flags | 0x6000;
		b->x = x;
		b->y = y;
		b->vx = vx;
		b->vy = vy;
		b->ax = 0;
		b->ay = 0;
		b->frame = frame;
		b->delay = 4;
		if (!(flags & 8)) {
			Snd_Stop(0xED);
			Snd_Play(0xED);
		}
	}
}
