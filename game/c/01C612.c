/* YoyoAcolyte_ActIdle100 - YOYOACOLYTE: the idle animation Tbl_YoyoAcolyteAnim for 100 frames,
 * facing the nearest live player, then restart the AI script (YoyoAcolyte_StateIdleRestart from
 * the next update). The state Init gives it, and its default state.
 *
 * self: the object.
 * Returns flags bit 3. */
#include "xmen2.h"

short YoyoAcolyte_ActIdle100(register YoyoAcolyte *self)
{
	Obj_ChangeAnim(self, CODEPTR(Tbl_YoyoAcolyteAnim));
	self->countdown = 100;
	SET_STATE(self, (StateFn)YoyoAcolyte_StateIdleRestart);
	return self->flags & 8;
}
