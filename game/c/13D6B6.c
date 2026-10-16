/* Ninja_ActIdleAnim - NINJA script action: the idle animation Tbl_NinjaAnimIdle, then the next
 * script step from the next update.
 *
 * self: the object. */
#include "xmen2.h"

void Ninja_ActIdleAnim(register Ninja *self)
{
	Obj_ChangeAnim(self, CODEPTR(Tbl_NinjaAnimIdle));
	AI_NEXT_STEP(self, self->ai, 0);
}
