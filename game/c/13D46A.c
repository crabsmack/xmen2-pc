/* Ninja_StateWaitLanding - NINJA state: wait (every frame) while flags bit $1000 is set and it
 * still has gravity (Ninja_ClipToSegs clears it on landing); then stop, play the landing
 * animation Tbl_NinjaAnimLand and go on with the next script step when it ends (delay $FF).
 *
 * self: the object.
 * Returns flags bit 3 while waiting; no value once landed. */
#include "xmen2.h"

short Ninja_StateWaitLanding(register Ninja *self)
{
	if (!(self->flags & 0x1000) || !self->ay) {
		STOP_MOVING(self);
		Obj_SetAnim(self, CODEPTR(Tbl_NinjaAnimLand));
		AI_NEXT_STEP(self, self->ai, 0xFF);
	} else {
		SET_STATE(self, Ninja_StateWaitLanding);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
