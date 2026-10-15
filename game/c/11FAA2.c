/* BioBalls_MotionRing - BIOBALLS motion function (XCLONE): the balls spiral out from the object,
 * ball i at radius i * motionSize and angle motionAngle + $19 i (256 a turn), the spiral turning
 * $19 a step; while ballFlags $100 is on they ease there (5 px a step) until all have arrived.
 *
 * self:  the object.
 * steps: steps to take (1, or 2 in two-player step mode).
 * The radius is in 1/256 px. While ballFlags $200 is on the spacing shrinks by $18 a step down
 * to $160. The list's end marker is not checked. Not layered. Returns 1: the balls are placed
 * (BioBalls_Move skips its path). */
#include "xmen2.h"

/* The ball sine at angle a (256 a turn): g_BioBallsSine holds half a turn (128 longs, 16
   fraction bits), the other half is its negative (as in BioBalls_MotionOrbit, _Helix). */
#define BALL_SIN(a)	((0xFF & (a)) < 0x80 ? (long)g_BioBallsSine[0x7F & (a)] \
				: -(long)g_BioBallsSine[0x7F & (a)])

char BioBalls_MotionRing(BioBalls *self, short steps)
{
	short count;
	char arrived = 1;
	char placed = 1;
	short tx, ty;
	short cx, cy;
	short r;
	BioBall *ball;
	short angle;
	short dx, dy;
	register unsigned short i;

	count = *self->ballList;
	self->ballFlags &= ~1;
	cx = self->x >> 8;
	cy = self->y >> 8;
	do {
		ball = (BioBall *)(self->ballList + 1);
		r = 0;
		angle = self->motionAngle;
		for (i = 0; i < count; i++, ball++, angle += 0x19, r += self->motionSize) {
			tx = (short)(cx + (r * (BALL_SIN(angle + 0x40) >> 8) >> 16));
			ty = (short)(cy + (r * (BALL_SIN(angle) >> 8) >> 16));
			if (self->ballFlags & 0x100) {
				dx = tx - ball->x;
				dy = ty - ball->y;
				if (dx || dy) {
					dx = MAX(-5, dx);
					dx = MIN(dx, 5);
					dy = MAX(-5, dy);
					dy = MIN(dy, 5);
					ball->x += dx;
					ball->y += dy;
					arrived = 0;
				}
			} else {
				ball->x = tx;
				ball->y = ty;
			}
		}
		if (arrived)
			self->ballFlags &= ~0x100;
		if (!(self->ballFlags & 0x100)) {
			if (self->ballFlags & 0x200) {
				if (self->motionSize > 0x160)
					self->motionSize -= 0x18;
				if (self->motionSize <= 0x160) {
					self->motionSize = 0x160;
					self->ballFlags &= ~0x200;
				}
			}
			self->motionAngle += 0x19;
			if (self->motionAngle >= 0x100)
				self->motionAngle -= 0x100;
		}
		steps--;
	} while (steps > 0);
	return placed;
}
