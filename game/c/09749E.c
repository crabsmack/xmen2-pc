/* XClone_StateBallsReturn - XCLONE state: when the rolling balls form has stopped, shake the screen
 * (Tbl_BeastQuakeBig) and roll it back the other way (BioBalls_MotionRoll, step negated,
 * BioBalls_WaitMotionDone); then XClone_StateWaitChild2.
 *
 * self: the clone (state set by XClone_ActBallsRoll). Returns flags & 8. */
#include "xmen2.h"

short XClone_StateBallsReturn(XClone *self)
{
	BioBalls *balls = self->child;

	if (BioBalls_IsDone(balls)) {
		g_CamShake = CODEPTR(Tbl_BeastQuakeBig);
		balls->motion = (BallMotionFn)BioBalls_MotionRoll;
		balls->motionSpeed = -balls->motionSpeed;
		balls->state = (StateFn)BioBalls_WaitMotionDone;
		balls->timer = 0;
		SET_STATE(self, XClone_StateWaitChild2);
		return self->flags & 8;
	}
	SET_STATE(self, XClone_StateBallsReturn);
	return self->flags & 8;
}
