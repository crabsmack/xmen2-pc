/* BigAcolyte_StateShootAnim - BIGACOLYTE state: play the shooting animation (Tbl_BigAcolyteAnim2),
 * and fire when it ends (BigAcolyte_StateShoot).
 *
 * self: the boss. Returns self->flags & 8. */
#include "xmen2.h"

short BigAcolyte_StateShootAnim(register BigAcolyte *self)
{
	Obj_SetAnim(self, CODEPTR(Tbl_BigAcolyteAnim2));
	SET_STATE_WAIT(self, BigAcolyte_StateShoot, 0xFF);
	return self->flags & 8;
}
