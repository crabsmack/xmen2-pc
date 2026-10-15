/* Barbarian_StateAttack0 - BARBARIAN attack 0: play Tbl_BarbarianAnim6, and at its end
 * Barbarian_StateAttackEnd.
 *
 * self: the object.
 * Returns self->flags & 8. */
#include "xmen2.h"

short Barbarian_StateAttack0(Barbarian *self)
{
	Obj_SetAnim(self, CODEPTR(Tbl_BarbarianAnim6));
	SET_STATE_WAIT(self, Barbarian_StateAttackEnd, 0xFF);
	return self->flags & 8;
}
