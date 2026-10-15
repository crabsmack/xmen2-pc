/* Barbarian_StateAttackEnd - BARBARIAN state: the attack animation is over - pose
 * (Tbl_BarbarianAnim4) and idle (Barbarian_StateIdle).
 *
 * self: the object.
 * Returns self->flags & 8. */
#include "xmen2.h"

short Barbarian_StateAttackEnd(Barbarian *self)
{
	Obj_SetAnim(self, CODEPTR(Tbl_BarbarianAnim4));
	SET_STATE(self, Barbarian_StateIdle);
	return self->flags & 8;
}
