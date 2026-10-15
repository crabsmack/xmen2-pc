/* Barbarian_StateAttack4 - BARBARIAN attack 4: play Tbl_BarbarianAnim10, and at its end
 * Barbarian_StateAttackEnd.
 *
 * self: the object.
 * Returns self->flags & 8. */
#include "xmen2.h"

short Barbarian_StateAttack4(Barbarian *self)
{
	Obj_SetAnim(self, CODEPTR(Tbl_BarbarianAnim10));
	SET_STATE_WAIT(self, Barbarian_StateAttackEnd, 0xFF);
	return self->flags & 8;
}
