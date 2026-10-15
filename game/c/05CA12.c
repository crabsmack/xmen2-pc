/* Tusk_StateChargeStop - TUSK after the charge: stop, idle animation, and walk back home after
 * 20 frames (Tusk_StateReturnStart).
 *
 * self: TUSK.
 * Returns 0. */
#include "xmen2.h"

short Tusk_StateChargeStop(Tusk *self)
{
	self->ax = 0;
	self->vx = 0;
	Obj_SetAnim(self, CODEPTR(Tbl_TuskAnim));
	self->state = Tusk_StateReturnStart;
	self->timer = 20;
	return 0;
}
