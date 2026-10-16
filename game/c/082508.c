/* DropBomb_StateDrop - DROPBOMB state: start falling: gravity $3C and the falling animation
 * (Tbl_DropBombAnimFall), then DropBomb_StateFall.
 *
 * self: the bomb. Returns flags & 8. */
#include "xmen2.h"

short DropBomb_StateDrop(DropBomb *self)
{
	self->ay = 0x3C;
	Obj_ChangeAnim(self, CODEPTR(Tbl_DropBombAnimFall));
	SET_STATE(self, DropBomb_StateFall);
	return self->flags & 8;
}
