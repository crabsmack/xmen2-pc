/* Ninja_ActCheckLeave - NINJA script action: with no leap left (leaveCount 0) switch to the leave
 * script Tbl_NinjaLeaveScript and run its first step, else the next script step; either from the
 * next update.
 *
 * self: the object. */
#include "xmen2.h"

void Ninja_ActCheckLeave(register Ninja *self)
{
	if (!self->leaveCount) {
		AiScript_Set(&self->ai, CODEPTR(Tbl_NinjaLeaveScript));
		AI_FIRST_STEP(self, self->ai, 0);
	} else {
		AI_NEXT_STEP(self, self->ai, 0);
	}
}
