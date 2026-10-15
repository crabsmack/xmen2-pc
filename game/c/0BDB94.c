/* Barbarian_StateIdleTimer - BARBARIAN state: when the idle countdown has run out, taunt
 * (Barbarian_StateTaunt); else keep the idle animation (Tbl_BarbarianAnim5) and watch the
 * players again (Barbarian_StateWatchP1).
 *
 * self: the object.
 * Returns self->flags & 8. */
#include "xmen2.h"

short Barbarian_StateIdleTimer(Barbarian *self)
{
	if (!self->countdown) {
		SET_STATE(self, Barbarian_StateTaunt);
		return self->flags & 8;
	}
	Obj_ChangeAnim(self, CODEPTR(Tbl_BarbarianAnim5));
	SET_STATE(self, Barbarian_StateWatchP1);
	return self->flags & 8;
}
