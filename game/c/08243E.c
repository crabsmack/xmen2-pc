/* DropBomb_StateStart - DROPBOMB first state: the waiting animation (Tbl_DropBombAnimWait), then
 * DropBomb_StateWaitInView.
 *
 * self: the bomb. Returns flags & 8. */
#include "xmen2.h"

short DropBomb_StateStart(DropBomb *self)
{
	Obj_ChangeAnim(self, CODEPTR(Tbl_DropBombAnimWait));
	SET_STATE(self, DropBomb_StateWaitInView);
	return self->flags & 8;
}
