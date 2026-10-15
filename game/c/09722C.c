/* XClone_ActBallsMotionA - XCLONE script action: the balls form $60 px above the arena floor, all
 * its balls on ball 0, orbiting (BioBalls_MotionOrbit from angle $40); go on with the script now.
 *
 * self: the clone (a step of its AI script; never runs in the corpus). */
#include "xmen2.h"

void XClone_ActBallsMotionA(XClone *self)
{
	BioBalls *balls = self->child;

	if (balls) {
		balls->y = (self->arenaFloor + -0x60) << 8;
		BioBalls_FillFromFirst(balls);
		balls->motion = (BallMotionFn)BioBalls_MotionOrbit;
		balls->motionAngle = 0x40;
		balls->motionSize = 0;
		balls->motionSpeed = 0;
	}
	AI_NEXT_STEP(self, self->ai, 0);
}
