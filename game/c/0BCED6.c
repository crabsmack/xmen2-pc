/* SavageMediumBio_ActWalk - SAVAGEMEDIUMBIO script action: walk the way it faces (speed $100,
 * the walk animation Tbl_Anim_12061C) for 60 frames (SavageMediumBio_StateWalk from the next
 * update).
 *
 * self: the object (a step of its AI script). Returns self->flags & 8. Never runs in the corpus. */
#include "xmen2.h"

short SavageMediumBio_ActWalk(register SavageMediumBio *self)
{
	Obj_ChangeAnim(self, CODEPTR(Tbl_Anim_12061C));
	self->countdown = 0x3C;
	self->vx = self->spr.flags & 0x10 ? -0x100 : 0x100;
	SET_STATE(self, SavageMediumBio_StateWalk);
	return self->flags & 8;
}
