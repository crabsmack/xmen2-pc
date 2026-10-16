/* EvilGambit_ActBallsPath - EVILGAMBIT AI-script action: the balls form follows its ball path
 * (BioBalls_MotionPath along Tbl_EvilGambitBallsMotionPath, from its start); go on with the
 * script now.
 *
 * self: the clone (a step of its AI script; never runs in the corpus).
 * Nothing happens to the balls when the clone has no BIOBALLS object. */
#include "xmen2.h"

void EvilGambit_ActBallsPath(EvilGambit *self)
{
	BioBalls *balls = self->child;

	if (balls) {
		balls->path = (struct PathPt *)CODEPTR(Tbl_EvilGambitBallsMotionPath);
		balls->motion = (BallMotionFn)BioBalls_MotionPath;
		balls->motionAngle = 0;
		balls->motionSize = 0;
	}
	AI_NEXT_STEP(self, self->ai, 0);
}
