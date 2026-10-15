/* BioBalls_MotionOrbit - BIOBALLS motion function (XCLONE): the balls circle the object on two
 * orbits, the even balls on the outer one, the odd balls on one a quarter its size; while
 * ballFlags $100 is on they ease there (5 px a step) until all have arrived. Never runs in the
 * corpus.
 *
 * self:  the object.
 * steps: steps to take (1, or 2 in two-player step mode).
 * Ball i sits at angle motionSize + 16 i (256 a turn) of an ellipse of radius r = $40 -
 * motionSpeed / 16 (odd balls r / 4): x = r sin(a) sin(t), y = -r cos(a), where t is the ball's
 * tag (the previous step's motionAngle; doubled for the odd balls), which then takes the
 * current motionAngle. Once in place both angles advance by motionSpeed / 16 a step (wrapping
 * past $100); while ballFlags $200 is on the speed grows by 8 a step until r is $20 or less.
 * Not layered. Returns 1: the balls are placed (BioBalls_Move skips its path). */
#include "xmen2.h"

/* The ball sine at angle a (256 a turn): g_BioBallsSine holds half a turn (128 longs, 16
   fraction bits), the other half is its negative (also in BioBalls_MotionRing, _Helix). */
#define BALL_SIN(a)	(((a) & 0xFF) < 0x80 ? (long)g_BioBallsSine[(a) & 0x7F] \
				: -(long)g_BioBallsSine[(a) & 0x7F])

char BioBalls_MotionOrbit(BioBalls *self, short steps)
{
	long r;
	short count;
	short tx, ty;
	char arrived = 1;
	char placed = 1;
	short cx, cy;
	short r2, r1;
	short ox, oy;
	short speed;
	BioBall *ball;
	short dx, dy;
	short phase = 0;
	register unsigned short i;

	count = *self->ballList;
	self->ballFlags &= ~1;
	do {
		ball = (BioBall *)(self->ballList + 1);
		phase = 0;
		cx = self->x >> 8;
		cy = self->y >> 8;
		r1 = 0x40 - (self->motionSpeed >> 4);
		r2 = r1 >> 2;
		for (i = 0; i < count; i++, ball++, phase += 0x10) {
			if (ball->x < 0 && ball->y < 0)
				break;
			if (i & 1) {
				r = r2;
				ox = (short)(BALL_SIN(phase + self->motionSize) * r >> 16);
				oy = (short)(BALL_SIN(phase + self->motionSize + 0x40) * r >> 16);
				tx = (short)(cx + (ox * BALL_SIN(ball->tag << 1) >> 16));
			} else {
				r = r1;
				ox = (short)(BALL_SIN(phase + self->motionSize) * r >> 16);
				oy = (short)(BALL_SIN(phase + self->motionSize + 0x40) * r >> 16);
				tx = (short)(cx + (ox * BALL_SIN(ball->tag) >> 16));
			}
			ball->tag = self->motionAngle;
			ty = cy - oy;
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
			speed = self->motionSpeed;
			if (self->ballFlags & 0x200) {
				if (r1 > 0x20) {
					self->motionSpeed += 8;
					speed = self->motionSpeed;
				} else
					self->ballFlags &= ~0x200;
			}
			self->motionAngle += speed >> 4;
			if (self->motionAngle > 0x100)
				self->motionAngle -= 0x100;
			self->motionSize += speed >> 4;
			if (self->motionSize > 0x100)
				self->motionSize -= 0x100;
		}
		steps--;
	} while (steps > 0);
	return placed;
}
