/* XClone_ActBallsRoll - XCLONE script action: roll the balls form the way the clone faces
 * (BioBalls_MotionRoll from angle $5A, step 10, BioBalls_WaitMotionDone), then roll it back
 * (XClone_StateBallsReturn). With no BIOBALLS object go on with the script on the next update.
 *
 * self: the clone (a step of its AI script).
 * Returns flags & 8 when rolling; nothing in particular otherwise. */
#include "xmen2.h"

short XClone_ActBallsRoll(XClone *self)
{
	BioBalls *balls = self->child;

	if (balls) {
		if (!(self->spr.flags & 0x10))
			balls->motionSpeed = -10;
		else
			balls->motionSpeed = 10;
		balls->motion = (BallMotionFn)BioBalls_MotionRoll;
		balls->done = 0;
		balls->motionSize = 0x5A;
		balls->state = (StateFn)BioBalls_WaitMotionDone;
		balls->timer = 0;
		SET_STATE(self, XClone_StateBallsReturn);
		return self->flags & 8;
	}
	AI_NEXT_STEP(self, self->ai, 0);
	NO_RETURN_VALUE
}
