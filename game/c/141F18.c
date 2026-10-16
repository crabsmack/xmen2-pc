/* TankLeft_StateDriveIn - TANKLEFT state while driving in (TankLeft_ActDriveIn): when its time
 * (countdown) is over, a tank left of the screen's middle (160 px) drives on and starts the
 * script Tbl_Tank_ScriptDrive (it drives away, and leaving the window removes it:
 * TankLeft_Deactivate); otherwise it goes on with its script.
 *
 * self: the object.
 * Returns self->flags & 8 while driving in, nothing otherwise. */
#include "xmen2.h"

short TankLeft_StateDriveIn(register TankLeft *self)
{
	if (self->countdown <= 0) {
		if ((self->x >> 8) < g_CameraX + 0xA0) {
			Obj_SetAnim(self, CODEPTR(Tbl_TankLeftDriveAnim));
			self->vx = self->spr.flags & 0x10 ? -self->speed : self->speed;
			AiScript_Set(&self->ai, CODEPTR(Tbl_Tank_ScriptDrive));
			AI_FIRST_STEP(self, self->ai, 0);
		} else {
			AI_NEXT_STEP(self, self->ai, 0);
		}
	} else {
		SET_STATE(self, TankLeft_StateDriveIn);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
