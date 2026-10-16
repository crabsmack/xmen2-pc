/* TankLeft_StateWaitDrive - TANKLEFT state after its wait action (TankLeft_ActWaitDrive): when
 * the wait (countdown) is over, a tank in sight (its box from 64 px left of the screen to 384 px
 * right of its left edge) drives (Tbl_TankLeftDriveAnim, the way it faces at its speed) and starts the
 * script Tbl_Tank_ScriptDriveIn; out of sight it goes on with its script.
 *
 * self: the object.
 * Returns self->flags & 8 while waiting, nothing otherwise.
 *
 * The AI script's jmp-thunk tables (separate in the disassembly): Tank_ScriptDriveIn.
 */
#include "xmen2.h"

short TankLeft_StateWaitDrive(register TankLeft *self)
{
	if (self->countdown <= 0) {
		if (self->boxLeft <= g_CameraX + 0x180 && self->boxRight >= g_CameraX - 0x40) {
			Obj_SetAnim(self, CODEPTR(Tbl_TankLeftDriveAnim));
			self->vx = self->spr.flags & 0x10 ? -self->speed : self->speed;
			AiScript_Set(&self->ai, CODEPTR(Tbl_Tank_ScriptDriveIn));
			AI_FIRST_STEP(self, self->ai, 0);
		} else {
			AI_NEXT_STEP(self, self->ai, 0);
		}
	} else {
		SET_STATE(self, TankLeft_StateWaitDrive);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
