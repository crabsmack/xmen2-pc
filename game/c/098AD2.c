/* EvilPsylocke_ActBallsPath - EVILPSYLOCKE AI-script action: the balls form follows its ball path
 * (BioBalls_MotionPath along Tbl_EvilPsylockeBallsMotionPath, from its start); go on with the
 * script on the next update.
 *
 * self: the clone (a step of its AI script).
 * Nothing happens to the balls when the clone has no BIOBALLS object. */
#include "xmen2.h"

void EvilPsylocke_ActBallsPath(EvilPsylocke *self)
{
	BioBalls *balls = self->child;

	if (balls) {
		balls->path = (struct PathPt *)CODEPTR(Tbl_EvilPsylockeBallsMotionPath);
		balls->motion = (BallMotionFn)BioBalls_MotionPath;
		balls->motionAngle = 0;
		balls->motionSize = 0;
	}
	AI_NEXT_STEP(self, self->ai, 0);
}
