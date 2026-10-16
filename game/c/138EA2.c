/* MaintRobot_StateStartVolley - MAINTROBOT state: back to the drive animation
 * (Tbl_MaintRobotAnimMove), load a volley of shots and fire (MaintRobot_StateFire from the next
 * update).
 *
 * self: the object.
 * Returns flags bit 3. */
#include "xmen2.h"

short MaintRobot_StateStartVolley(register MaintRobot *self)
{
	Obj_SetAnim(self, CODEPTR(Tbl_MaintRobotAnimMove));
	self->shotsLeft = self->volley;
	SET_STATE(self, MaintRobot_StateFire);
	return self->flags & 8;
}
