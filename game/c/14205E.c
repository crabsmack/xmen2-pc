/* TankLeft_ActDrive - TANKLEFT script action: drive (Tbl_TankLeftDriveAnim, the way it faces at its
 * speed), and the next script step in driveTime frames.
 *
 * self: the object. */
#include "xmen2.h"

void TankLeft_ActDrive(register TankLeft *self)
{
	Obj_SetAnim(self, CODEPTR(Tbl_TankLeftDriveAnim));
	self->vx = self->spr.flags & 0x10 ? -self->speed : self->speed;
	AI_NEXT_STEP(self, self->ai, self->driveTime);
}
