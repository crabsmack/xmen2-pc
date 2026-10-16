/* XClone_ActBallsMotionC - XCLONE script action: all the balls on ball 0, in a turning double helix
 * (BioBalls_MotionHelix, spacing $900, speed 10); go on with the script on the next update.
 *
 * self: the clone (a step of its AI script). */
#include "xmen2.h"

void XClone_ActBallsMotionC(XClone *self)
{
	BioBalls *balls = self->child;

	if (balls) {
		BioBalls_FillFromFirst(balls);
		balls->motion = (BallMotionFn)BioBalls_MotionHelix;
		balls->motionAngle = 0;
		balls->motionSize = 0x900;
		balls->motionSpeed = 10;
	}
	AI_NEXT_STEP(self, self->ai, 0);
}
