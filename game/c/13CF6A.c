/* Ninja_ClipToSegs - keep an airborne NINJA out of floor segments: after a move, if its new body
 * box meets a segment (Ninja_FindSegInBox, around the old box's centre), put it back beside or
 * on / under the segment and stop the motion that ran into it.
 *
 * self: the ninja (just moved by Ninja_Move).  prevX, prevY: its position before the move (pixels).
 * Only the sides the old box was outside of count. Horizontally (moving right: its box's right
 * end 1 px left of the segment's left end, the smaller of the two; moving left: 1 px right of its
 * right end) the x speed and acceleration stop. Vertically, falling: its box's bottom (or y, when
 * lower) on the segment's top, no y speed or gravity (Ninja_StateWaitLanding waits for that);
 * rising: its top under the segment's bottom; either way vy stops. Then also: x changed stops
 * the x motion, y moved up stops vy and gravity, down vy. The boxes are updated
 * (Obj_UpdateBoxes) before the test and after the move. */
#include "xmen2.h"

void Ninja_ClipToSegs(register Ninja *self, short prevX, short prevY)
{
	register short oldLeft = self->boxLeft;
	short oldRight = self->boxRight;
	short oldTop = self->boxTop;
	short oldBottom = self->boxBottom;
	register Seg *seg;

	Obj_UpdateBoxes(self);
	if (seg = Ninja_FindSegInBox(oldLeft + ((oldRight - oldLeft) >> 1),
			oldTop + ((oldBottom - oldTop) >> 1), 0, (self->vy >> 8) + 1,
			self->boxLeft, self->boxTop, self->boxRight, self->boxBottom)) {
		short segLeft, segRight, segTop, segBottom;
		register short newY, newX;
		short x = self->x >> 8;
		register short y = self->y >> 8;
		short width = self->boxRight - self->boxLeft;
		short height = MAX(self->boxBottom, y) - self->boxTop;
		short dyTop;

		segLeft = MIN(seg->x1, seg->x2);
		segRight = MAX(seg->x1, seg->x2);
		segTop = MIN(seg->y1, seg->y2);
		segBottom = MAX(seg->y1, seg->y2);
		newX = self->boxLeft;
		if (oldRight < segLeft || oldLeft > segRight) {
			if (x > prevX) {
				newX = MIN(self->boxRight, segLeft);
				newX -= 1;
				newX -= width;
			} else if (x < prevX) {
				newX = MAX(self->boxLeft, segRight);
				newX += 1;
			}
			self->ax = 0;
			self->vx = 0;
		}
		newX += x - self->boxLeft;
		dyTop = y - self->boxTop;
		newY = y;
		if (oldBottom < segTop || oldTop > segBottom) {
			if (y > prevY) {
				short t;

				t = segTop - height;
				newY = t + dyTop;
				self->vy = 0;
				self->ay = 0;
			} else if (y < prevY)
				newY = dyTop + MAX(self->boxTop, segBottom);
			self->vy = 0;
		}
		if (newX != x) {
			self->ax = 0;
			self->vx = 0;
		}
		if (newY < y) {
			self->vy = 0;
			self->ay = 0;
		} else if (newY > y)
			self->vy = 0;
		self->x = (long)newX << 8;
		self->y = (long)newY << 8;
		Obj_UpdateBoxes(self);
	}
}
