/* Cam_SpringAxis - move one camera coordinate toward the dead zone's edge with a springy speed.
 *
 * target, prev: the target's coordinate now and last frame; lo, hi: the dead zone's edges (screen
 * pixels); catchUp: how much more than the target the camera may move per frame; maxAccel: the
 * largest speed change per frame; pos: the camera coordinate; speed: its speed (8 fraction bits);
 * accel: the speed change per frame; accelStep: accel's change per frame.
 * Nothing while the target stands still. The camera aims to put the target on the edge it moves
 * toward (lo moving forward, hi moving back). Moving toward the aim: accel grows by accelStep
 * (up to maxAccel), speed by accel, and the move (speed >> 8) is at most the distance and the
 * target's own move plus catchUp, and never against the target's motion. A speed the wrong way
 * is stopped instead (speed and accel 0, accelStep turned the right way).
 * Not byte-identical (executed: verify): the ROM has maxAccel in d4 and want/v in d5, and keeps
 * the second branch's lim in d5 (the first branch's lim and step on the stack); ~10 register /
 * declaration-order variants came within 155 bytes. */
#include "xmen2.h"

void Cam_SpringAxis(register short target, short prev, short lo, short hi, short catchUp,
	register short maxAccel, s16 *pos, register s16 *speed, s16 *accel, register s16 *accelStep)
{
	short lim, step;
	register short moved = target - prev;

	if (moved) {
		register short want = moved > 0 ? target - lo : target - hi;
		register short dist;
		short v;

		dist = want - *pos;
		if (dist > 0) {
			if (*speed < 0) {
				*speed = 0;
				*accel = 0;
				if (*accelStep < 0)
					*accelStep = -*accelStep;
				return;
			}
			if (moved <= 0)
				return;
			moved += catchUp;
			lim = MIN(moved, dist);
			v = *accel + *accelStep;
			if (v > maxAccel)
				v = maxAccel;
			*accel = v;
			*speed += v;
			step = *speed >> 8;
			if (step > lim) {
				step = lim;
				*speed = lim << 8;
			}
			*pos += step;
		} else {
			if (*speed > 0) {
				*speed = 0;
				*accel = 0;
				if (*accelStep > 0)
					*accelStep = -*accelStep;
				return;
			}
			if (moved >= 0)
				return;
			moved -= catchUp;
			lim = MAX(moved, dist);
			v = *accel + *accelStep;
			if (v < -maxAccel)
				v = -maxAccel;
			*accel = v;
			*speed += v;
			step = *speed >> 8;
			if (step < lim) {
				step = lim;
				*speed = lim << 8;
			}
			*pos += step;
		}
	}
}