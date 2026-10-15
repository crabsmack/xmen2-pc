/* Tuskette_StateWalk - TUSKETTE walker on the move: at a ledge (50 px ahead) turn round and
 * stop, or stop when the walk's countdown has run out; then idle 120 frames and walk again
 * (Tuskette_StateWalkStart).
 *
 * self: the object. Never runs in the corpus.
 * Returns self->flags & 8 when it stops, nothing while walking. */
#include "xmen2.h"

short Tuskette_StateWalk(Tuskette *self)
{
	if (Seg_WouldLeaveEnd(self, 50)) {
		self->spr.flags ^= 0x10;
		self->ax = 0;
		self->vx = 0;
		Obj_SetAnim(self, CODEPTR(Tbl_TusketteAnim));
		SET_STATE_WAIT(self, Tuskette_StateWalkStart, 120);
		return self->flags & 8;
	} else if (self->countdown <= 0) {
		self->ax = 0;
		self->vx = 0;
		Obj_SetAnim(self, CODEPTR(Tbl_TusketteAnim));
		SET_STATE_WAIT(self, Tuskette_StateWalkStart, 120);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
