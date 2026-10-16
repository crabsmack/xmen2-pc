/* MaintRobot_StateStop - MAINTROBOT state: stop, play the stop animation Tbl_MaintRobotAnimStop
 * and start a volley 30 frames later (MaintRobot_StateStartVolley).
 *
 * self: the object.
 * Returns flags bit 3. */
#include "xmen2.h"

short MaintRobot_StateStop(register MaintRobot *self)
{
	STOP_MOVING(self);
	Obj_SetAnim(self, CODEPTR(Tbl_MaintRobotAnimStop));
	SET_STATE_WAIT(self, MaintRobot_StateStartVolley, 30);
	return self->flags & 8;
}
