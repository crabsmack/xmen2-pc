/* Barbarian_StateAttack1 - BARBARIAN attack 1: play Tbl_BarbarianAnim7, and at its end
 * Barbarian_StateAttackEnd.
 *
 * self: the object.
 * Returns self->flags & 8. */
#include "xmen2.h"

short Barbarian_StateAttack1(Barbarian *self)
{
	Obj_SetAnim(self, CODEPTR(Tbl_BarbarianAnim7));
	SET_STATE_WAIT(self, Barbarian_StateAttackEnd, 0xFF);
	return self->flags & 8;
}
