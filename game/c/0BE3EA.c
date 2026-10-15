/* Barbarian_StateAttack2 - BARBARIAN attack 2: play Tbl_BarbarianAnim8, and at its end
 * Barbarian_StateAttackEnd.
 *
 * self: the object.
 * Returns self->flags & 8. */
#include "xmen2.h"

short Barbarian_StateAttack2(Barbarian *self)
{
	Obj_SetAnim(self, CODEPTR(Tbl_BarbarianAnim8));
	SET_STATE_WAIT(self, Barbarian_StateAttackEnd, 0xFF);
	return self->flags & 8;
}
