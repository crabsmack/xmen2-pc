/* Tuskette_StateExitLeft - TUSKETTE spawned by TUSK (kind 2), first state: face left, walk at
 * $200 off TUSK's platform (Tuskette_StateExitWalk).
 *
 * self: the object.
 * Returns self->flags & 8. */
#include "xmen2.h"

short Tuskette_StateExitLeft(Tuskette *self)
{
	self->spr.flags |= 0x10;
	self->vx = -0x200;
	Obj_SetAnim(self, CODEPTR(Tbl_TusketteAnim2));
	SET_STATE(self, Tuskette_StateExitWalk);
	return self->flags & 8;
}
