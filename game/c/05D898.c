/* Tuskette_StateRunRight - TUSKETTE runner: run right with the walk animation, from a quarter of
 * `speed` up to `speed` (acceleration 40); flags6 bit $4000 off; until its right turning point
 * (Tuskette_StateRunRightUntil).
 *
 * self: the object.
 * Returns self->flags & 8. */
#include "xmen2.h"

short Tuskette_StateRunRight(Tuskette *self)
{
	Obj_SetAnim(self, CODEPTR(Tbl_TusketteAnim2));
	self->ax = 40;
	self->vx = self->speed >> 2;
	self->maxVx = self->speed;
	self->flags6 &= ~0x4000;
	SET_STATE(self, Tuskette_StateRunRightUntil);
	return self->flags & 8;
}
