/* Ninja_ActPause30 - NINJA script action (the leave script's second step): stop, the idle
 * animation Tbl_NinjaAnimIdle, then the next script step in 30 frames.
 *
 * self: the object. Never runs in the corpus. */
#include "xmen2.h"

void Ninja_ActPause30(register Ninja *self)
{
	STOP_MOVING(self);
	Obj_ChangeAnim(self, CODEPTR(Tbl_NinjaAnimIdle));
	AI_NEXT_STEP(self, self->ai, 0x1E);
}
