/* Barbarian_StateAttack3 - BARBARIAN attack 3: play Tbl_BarbarianAnim9, and at its end
 * Barbarian_StateAttackEnd.
 *
 * self: the object.
 * Returns self->flags & 8. */
#include "xmen2.h"

short Barbarian_StateAttack3(Barbarian *self)
{
	Obj_SetAnim(self, CODEPTR(Tbl_BarbarianAnim9));
	SET_STATE_WAIT(self, Barbarian_StateAttackEnd, 0xFF);
	return self->flags & 8;
}
