/* BioBalls_MotionHelix - BIOBALLS motion function (XCLONE): the balls stand in a turning double
 * helix up from the object: two strands of half the balls each, half a turn apart, each ball
 * motionSize (1/256 px) above the one before and $20 further round (256 a turn); while ballFlags
 * $100 is on they ease there (5 px a step) until all have arrived.
 *
 * self:  the object.
 * steps: steps to take (1, or 2 in two-player step mode).
 * x = the object's + radius sin(angle + 90 degrees) with radius = 1.75 motionSize (1/256 px,
 * taken when the motion starts); balls on the near side (angle $00-$7F, $100-$17F) are tagged
 * in front, drawn layered (ballFlags bit 0). Once in place the helix turns by motionSpeed & $FF
 * a step; while ballFlags $200 is on the spacing grows by $40 and the speed by 1 a step up to
 * a spacing of $2800. Returns 1: the balls are placed (BioBalls_Move skips its path). */
#include "xmen2.h"

/* The ball sine at angle a (256 a turn): g_BioBallsSine holds half a turn (128 longs, 16
   fraction bits), the other half is its negative (as in BioBalls_MotionOrbit, _Ring). */
#define BALL_SIN(a)	((0xFF & (a)) < 0x80 ? (long)g_BioBallsSine[0x7F & (a)] \
				: -(long)g_BioBallsSine[0x7F & (a)])

char BioBalls_MotionHelix(BioBalls *self, short steps)
{
	short count;
	short tx, ty;
	char placed = 1;
	short cx;
	long y;
	char arrived;
	short spacing, radius;
	BioBall *ball;
	short dx, dy;
	short angle;
	register unsigned short i;

	count = *self->ballList;
	self->ballFlags |= 1;
	arrived = 1;
	cx = self->x >> 8;
	spacing = self->motionSize;
	radius = self->motionSize + (self->motionSize >> 1) + (self->motionSize >> 2);
	do {
		y = self->y;
		ball = (BioBall *)(self->ballList + 1);
		angle = self->motionAngle;
		for (i = 0; i < count; i++, ball++, angle += 0x20, y -= spacing) {
			if (i == count >> 1) {		/* the second strand */
				angle = self->motionAngle + 0x80;
				y = self->y;
			}
			tx = (short)(cx + (radius * BALL_SIN(angle + 0x40) >> 24));
			ty = (short)(y >> 8);
			if (angle >= 0 && angle < 0x80 || angle >= 0x100 && angle < 0x180)
				ball->tag |= 1;
			else
				ball->tag &= ~1;
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
			self->motionAngle += self->motionSpeed & 0xFF;
			if (self->ballFlags & 0x200) {
				if (self->motionSize < 0x2800) {
					self->motionSize += 0x40;
					self->motionSpeed++;
				} else
					self->ballFlags &= ~0x200;
			}
			if (self->motionAngle > 0x100)
				self->motionAngle -= 0x100;
		}
		steps--;
	} while (steps > 0);
	return placed;
}
