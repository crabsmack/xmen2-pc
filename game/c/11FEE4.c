/* BioBalls_MotionPath - BIOBALLS motion function: the balls stand on the path's points from
 * point motionAngle on (one per ball, the path restarting at a point with x >= 1000), from the
 * object's position, x swaying with sin(motionSize); while ballFlags $100 is on they ease there
 * (5 px a step) until all have arrived.
 *
 * self:  the object (with a path).
 * steps: steps to take (1, or 2 in two-player step mode).
 * Once in place each step moves the balls one point on (back to point 0 at the path's end) and
 * the sway 5 degrees. Not layered. Returns 1: the balls are placed (BioBalls_Move skips its
 * path easing). */
#include "xmen2.h"

char BioBalls_MotionPath(BioBalls *self, short steps)
{
	short count;
	short tx;
	char placed = 1;
	short cx, cy;
	char arrived;
	PathPt *pt;
	BioBall *ball;
	short ty, dx, dy;
	register unsigned short i;

	count = *self->ballList;
	self->ballFlags &= ~1;
	arrived = 1;
	cx = self->x >> 8;
	cy = self->y >> 8;
	do {
		ball = (BioBall *)(self->ballList + 1);
		pt = self->path + self->motionAngle;
		for (i = 0; i < count; i++, ball++, pt++) {
			if (ball->x < 0 && ball->y < 0)
				break;
			if (pt->x >= 1000)
				pt = self->path;
			tx = (short)(cx + (pt->x * Math_SinDeg(self->motionSize) >> 16));
			ty = cy + pt->y;
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
			self->motionAngle++;
			pt = self->path + self->motionAngle;
			if (pt->x >= 1000)
				self->motionAngle = 0;
			self->motionSize += 5;
			if (self->motionSize > 360)
				self->motionSize -= 360;
		}
		steps--;
	} while (steps > 0);
	return placed;
}
