/* XClone_ActBallsMotionB - XCLONE script action: the balls form $60 px above the arena floor, all
 * its balls on ball 0, in a shrinking ring (BioBalls_MotionRing, radius $600); go on with the
 * script now.
 *
 * self: the clone (a step of its AI script; never runs in the corpus). */
#include "xmen2.h"

void XClone_ActBallsMotionB(XClone *self)
{
	BioBalls *balls = self->child;

	if (balls) {
		balls->y = (self->arenaFloor + -0x60) << 8;
		BioBalls_FillFromFirst(balls);
		balls->motion = (BallMotionFn)BioBalls_MotionRing;
		balls->motionAngle = 0;
		balls->motionSize = 0x600;
	}
	AI_NEXT_STEP(self, self->ai, 0);
}
