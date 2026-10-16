/* BioSentinel_StateDropFlip - BIOSENTINEL dropping from the ceiling: turn right side up (v-flip
 * off) with the falling-down animation, keeping the middle of its body box where it was (y
 * moves by the box's shift, Obj_UpdateBox); BioSentinel_StateFalling next frame.
 *
 * self: the sentinel. Returns self->flags & 8. */
#include "xmen2.h"

short BioSentinel_StateDropFlip(register BioSentinel *self)
{
	register long mid = (self->boxTop + self->boxBottom) << 7;

	self->spr.flags &= ~8;
	Obj_SetAnim(self, CODEPTR(Tbl_Anim_12067E));
	Obj_UpdateBox(self);
	self->y += mid - ((self->boxTop + self->boxBottom) << 7);
	SET_STATE(self, BioSentinel_StateFalling);
	return self->flags & 8;
}
