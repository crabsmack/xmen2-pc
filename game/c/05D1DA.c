/* Tuskette_StateWalkStart - TUSKETTE walker: turn round at a ledge (50 px ahead), else face the
 * nearest player; walk that way at `speed` for up to 300 frames (Tuskette_StateWalk).
 *
 * self: the object. Never runs in the corpus.
 * Returns self->flags & 8. */
#include "xmen2.h"

short Tuskette_StateWalkStart(Tuskette *self)
{
	XPlayer *p;

	if (Seg_WouldLeaveEnd(self, 50))
		self->spr.flags ^= 0x10;
	else {
		p = Player_Nearest((Actor *)self);
		if (p->x < self->x)
			self->spr.flags |= 0x10;
		else
			self->spr.flags &= ~0x10;
	}
	self->vx = self->spr.flags & 0x10 ? -self->speed : self->speed;
	self->countdown = 300;
	Obj_SetAnim(self, CODEPTR(Tbl_TusketteAnim2));
	SET_STATE(self, Tuskette_StateWalk);
	return self->flags & 8;
}
