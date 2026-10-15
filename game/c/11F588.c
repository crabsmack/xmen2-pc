/* BioBalls_MotionGather - BIOBALLS motion function: pull every ball toward one point, the anchor
 * (object + pathDx - 8, object + pathDy); the motion ends (0) once all are there.
 *
 * self:  the object.
 * steps: steps to take (1, or 2 in two-player step mode).
 * In x a ball 8 px or more away moves an eighth of the way, closer it jumps there; in y at most
 * 5 px a step. Not layered. Returns 1: the balls are placed (BioBalls_Move skips its path). */
#include "xmen2.h"

char BioBalls_MotionGather(BioBalls *self, short steps)
{
	short count;
	short tx, ty;
	char arrived = 1;
	char placed = 1;
	BioBall *ball;
	short dx, dy;
	register unsigned short i;

	tx = (self->x >> 8) + self->pathDx - 8;
	ty = (self->y >> 8) + self->pathDy;
	count = *self->ballList;
	self->ballFlags &= ~1;
	do {
		ball = (BioBall *)(self->ballList + 1);
		for (i = 0; i < count; i++, ball++) {
			if (ball->x < 0 && ball->y < 0)
				break;
			dx = tx - ball->x;
			dy = ty - ball->y;
			if (dx || dy) {
				if (ABS(dx) >= 8)
					dx >>= 3;
				dy = MAX(-5, dy);
				dy = MIN(dy, 5);
				ball->x += dx;
				ball->y += dy;
				arrived = 0;
			}
		}
		steps--;
	} while (steps > 0);
	if (arrived)
		self->motion = 0;
	return placed;
}
