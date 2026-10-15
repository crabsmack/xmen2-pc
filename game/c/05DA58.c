/* Tuskette_StateExitRight - TUSKETTE spawned by TUSK (kind 3), first state: face right, walk at
 * $200 off TUSK's platform (Tuskette_StateExitWalk).
 *
 * self: the object.
 * Returns self->flags & 8. */
#include "xmen2.h"

short Tuskette_StateExitRight(Tuskette *self)
{
	self->spr.flags &= ~0x10;
	self->vx = 0x200;
	Obj_SetAnim(self, CODEPTR(Tbl_TusketteAnim2));
	SET_STATE(self, Tuskette_StateExitWalk);
	return self->flags & 8;
}
