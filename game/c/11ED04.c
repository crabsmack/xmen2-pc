/* BioBalls_Move - BIOBALLS Move (class slot +$4E): the balls move, not the object. First the
 * motion function (BioBalls_Motion*); unless it placed the balls, ease each ball toward its
 * point of the path, at most 5 px a step on each axis; once all are there the path is done
 * (0).
 *
 * self:  the object.
 * steps: steps to take (1, or 2 in two-player step mode).
 * Point i is for ball i (the path restarts at a point with x >= 1000), from the anchor = the
 * position + pathDx/pathDy (ballFlags bit 3: the origin: absolute points), mirrored in x when
 * ballFlags bit 1 is set, in y when bit 2 is. The list ends at its count or a ball at (-1, -1).
 * Returns early, the path untouched, without a ball list or when the motion placed the balls. */
#include "xmen2.h"

void BioBalls_Move(BioBalls *self, short steps)
{
	short count;
	short ax, ay;
	short tx;
	char arrived = 1;
	char placed;
	BioBall *ball;
	PathPt *pt;
	short ty, dx, dy;
	register unsigned short i;

	if (!self->ballList)
		return;
	if (self->motion && (placed = (*self->motion)(self, steps)))
		return;
	if (!self->path)
		return;
	count = *self->ballList;
	if (self->ballFlags & 8) {
		ax = 0;
		ay = 0;
	} else {
		ax = (self->x >> 8) + self->pathDx;
		ay = (self->y >> 8) + self->pathDy;
	}
step:	/* each step (a goto loop: the ROM gives placed no register) */
	ball = (BioBall *)(self->ballList + 1);
	pt = self->path;
	for (i = 0; i < count; i++, ball++, pt++) {
		if (ball->x < 0 && ball->y < 0)
			break;
		if (pt->x >= 1000)
			pt = self->path;
		tx = ax + (!(self->ballFlags & 2) ? pt->x : -pt->x);
		ty = ay + (!(self->ballFlags & 4) ? pt->y : -pt->y);
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
	}
	steps--;
	if (steps > 0)
		goto step;
	if (arrived)
		self->path = 0;
}
