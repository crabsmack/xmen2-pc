/* Barbarian_StateWalkStart - BARBARIAN state: start walking in the facing direction at walkSpeed
 * (walk animation Tbl_BarbarianAnim3, no vertical motion, flags6 bit $4000 off):
 * Barbarian_StateWalkRight or Barbarian_StateWalkLeft.
 *
 * self: the object.
 * Returns self->flags & 8. */
#include "xmen2.h"

short Barbarian_StateWalkStart(Barbarian *self)
{
	Obj_ChangeAnim(self, CODEPTR(Tbl_BarbarianAnim3));
	self->ay = 0;
	self->vy = 0;
	self->flags6 &= ~0x4000;
	if (!(self->spr.flags & 0x10)) {
		self->vx = self->walkSpeed;
		SET_STATE(self, Barbarian_StateWalkRight);
		return self->flags & 8;
	}
	self->vx = -self->walkSpeed;
	SET_STATE(self, Barbarian_StateWalkLeft);
	return self->flags & 8;
}
