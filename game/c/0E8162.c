/* HoloBolt_StateEnd - HOLOBOLT state: the end animation (Tbl_HoloBoltAnimEnd), then
 * HoloBolt_StateDone from the next update.
 *
 * self: the bolt.
 * Returns self->flags & 8. */
#include "xmen2.h"

short HoloBolt_StateEnd(register HoloBolt *self)
{
	Obj_SetAnim(self, CODEPTR(Tbl_HoloBoltAnimEnd));
	SET_STATE(self, (StateFn)HoloBolt_StateDone);
	return self->flags & 8;
}
