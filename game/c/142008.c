/* TankLeft_ActDriveIn - TANKLEFT script action: drive (Tbl_TankLeftDriveAnim, the way it faces at its
 * speed) for driveTime frames, watching for the screen's middle (TankLeft_StateDriveIn).
 *
 * self: the object.  Returns self->flags & 8. */
#include "xmen2.h"

short TankLeft_ActDriveIn(register TankLeft *self)
{
	Obj_SetAnim(self, CODEPTR(Tbl_TankLeftDriveAnim));
	self->vx = self->spr.flags & 0x10 ? -self->speed : self->speed;
	self->countdown = self->driveTime;
	SET_STATE(self, TankLeft_StateDriveIn);
	return self->flags & 8;
}
