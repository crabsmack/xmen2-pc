/* MaintRobot_StateResume - MAINTROBOT state (Init's, and the end of a volley): the drive
 * animation Tbl_MaintRobotAnimMove, fireDelay frames before it may fire again, then chase the
 * player (MaintRobot_StateChase) or, in patrol mode, patrol again at patrolVx
 * (MaintRobot_StatePatrol), from the next update.
 *
 * self: the object.
 * Returns flags bit 3. */
#include "xmen2.h"

short MaintRobot_StateResume(register MaintRobot *self)
{
	Obj_SetAnim(self, CODEPTR(Tbl_MaintRobotAnimMove));
	self->countdown = self->fireDelay;
	if (!self->patrol) {
		SET_STATE(self, MaintRobot_StateChase);
		return self->flags & 8;
	} else {
		self->vx = self->patrolVx;
		SET_STATE(self, MaintRobot_StatePatrol);
		return self->flags & 8;
	}
}
