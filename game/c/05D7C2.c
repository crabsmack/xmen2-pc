/* Tuskette_StateRestIdle - TUSKETTE runner (kind 4 and others), first state: idle animation,
 * then wait for a player to charge at (Tuskette_StateWaitCharge).
 *
 * self: the object.
 * Returns self->flags & 8. */
#include "xmen2.h"

short Tuskette_StateRestIdle(Tuskette *self)
{
	Obj_SetAnim(self, CODEPTR(Tbl_TusketteAnim));
	SET_STATE(self, Tuskette_StateWaitCharge);
	return self->flags & 8;
}
