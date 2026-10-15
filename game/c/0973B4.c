/* XClone_ActBallsMotionD - XCLONE script action: gather the balls (BioBalls_MotionGather) to a
 * point $C0 px up from the centre of their bounding box (BioBalls_UpdateBox), waiting in
 * BioBalls_WaitMotionDone; then wait for them (XClone_StateWaitChild).
 *
 * self: the clone (a step of its AI script). Returns flags & 8. */
#include "xmen2.h"

short XClone_ActBallsMotionD(XClone *self)
{
	register BioBalls *balls = self->child;
	short px;

	if (balls) {
		px = balls->x >> 8;
		balls->motion = (BallMotionFn)BioBalls_MotionGather;
		BioBalls_UpdateBox(balls);
		balls->pathDx = ((balls->boxRight + balls->boxLeft) >> 1) - px;
		balls->pathDy = -0xC0;
		balls->done = 0;
		balls->state = (StateFn)BioBalls_WaitMotionDone;
		balls->timer = 0;
	}
	SET_STATE(self, XClone_StateWaitChild);
	return self->flags & 8;
}
