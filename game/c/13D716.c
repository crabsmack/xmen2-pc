/* Ninja_ActWait - NINJA script action (the intro script's first step, the combat script's last):
 * stop, the idle animation Tbl_NinjaAnimIdle, then the next script step in waitTime frames.
 *
 * self: the object. */
#include "xmen2.h"

void Ninja_ActWait(register Ninja *self)
{
	STOP_MOVING(self);
	Obj_ChangeAnim(self, CODEPTR(Tbl_NinjaAnimIdle));
	AI_NEXT_STEP(self, self->ai, self->waitTime);
}
