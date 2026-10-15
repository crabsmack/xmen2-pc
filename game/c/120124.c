/* BioBalls_MotionRoll - BIOBALLS motion function (XCLONE): the ball chain rolls over through a
 * half circle: point i of the path (one per ball, restarting at a point with x >= 1000) turned
 * by motionSize degrees about the object (x = cx + |y| cos + x, y = cy + y sin); while ballFlags
 * $100 is on they ease there (5 px a step) until all have arrived.
 *
 * self:  the object (with a path).
 * steps: steps to take (1, or 2 in two-player step mode).
 * Once in place the angle moves by motionSpeed a step; past 0 or 180 it stops there, the motion
 * ends (0) and a puff (BioBalls_SpawnPuff) appears where the chain landed: the farthest ball's
 * distance to the right of the object (at 0) or to the left (at 180, h-flipped). Not layered.
 * Returns 1: the balls are placed (BioBalls_Move skips its path easing). */
#include "xmen2.h"

char BioBalls_MotionRoll(BioBalls *self, short steps)
{
	short count;
	short tx, ty;
	char placed = 1;
	short cx, cy;
	char arrived;
	short maxR = 0;
	PathPt *pt;
	BioBall *ball;
	short r;
	short dx, dy;
	register unsigned short i;

	count = *self->ballList;
	self->ballFlags &= ~1;
	arrived = 1;
	cx = self->x >> 8;
	cy = self->y >> 8;
	do {
		ball = (BioBall *)(self->ballList + 1);
		pt = self->path;
		for (i = 0; i < count; i++, ball++, pt++) {
			if (ball->x < 0 && ball->y < 0)
				break;
			if (pt->x >= 1000)
				pt = self->path;
			r = ABS(pt->y);
			maxR = MAX(r, maxR);
			tx = (short)(cx + (r * Math_CosDeg(self->motionSize) >> 16) + pt->x);
			ty = (short)(cy + (pt->y * Math_SinDeg(self->motionSize) >> 16));
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
			self->motionSize += self->motionSpeed;
			if (self->motionSize < 0 || self->motionSize > 180) {
				if (self->motionSize < 0)
					self->motionSize = 0;
				else
					self->motionSize = 180;
				self->motion = 0;
				r = (short)((self->x >> 8) + (self->motionSize == 0 ? maxR : -maxR));
				BioBalls_SpawnPuff(r, self->y >> 8, self->motionSize == 180 ? 1 : 0);
			}
		}
		steps--;
	} while (steps > 0);
	return placed;
}
