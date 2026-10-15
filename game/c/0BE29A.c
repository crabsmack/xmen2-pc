/* Barbarian_StateGiveUp - BARBARIAN state: the target can't be reached - idle animation
 * (Tbl_BarbarianAnim5) and idle (Barbarian_StateIdle).
 *
 * self: the object. Never runs in the corpus.
 * Returns self->flags & 8. */
#include "xmen2.h"

short Barbarian_StateGiveUp(Barbarian *self)
{
	Obj_ChangeAnim(self, CODEPTR(Tbl_BarbarianAnim5));
	SET_STATE(self, Barbarian_StateIdle);
	return self->flags & 8;
}
