/* TankLeft_ActWaitDrive - TANKLEFT script action: wait driveTime frames, then drive if in sight
 * (TankLeft_StateWaitDrive).
 *
 * self: the object.  Returns self->flags & 8. */
#include "xmen2.h"

short TankLeft_ActWaitDrive(register TankLeft *self)
{
	self->countdown = self->driveTime;
	SET_STATE(self, TankLeft_StateWaitDrive);
	return self->flags & 8;
}
