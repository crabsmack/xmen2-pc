/* HoloBolt_StateAppear - HOLOBOLT's first state: the appear animation (Tbl_HoloBoltAnimAppear);
 * HoloBolt_StateActivate when it ends (timer $FF).
 *
 * self: the bolt.
 * Returns self->flags & 8. */
#include "xmen2.h"

short HoloBolt_StateAppear(register HoloBolt *self)
{
	Obj_SetAnim(self, CODEPTR(Tbl_HoloBoltAnimAppear));
	SET_STATE_WAIT(self, HoloBolt_StateActivate, 0xFF);
	return self->flags & 8;
}
