/* BigBio_ActFaceIdle90 - BIGBIO script action: face the way +$B6 says, idle animation, next step
 * of the script in 90 frames.
 *
 * self: the boss. Never runs in the corpus. */
#include "xmen2.h"

void BigBio_ActFaceIdle90(register BigBio *self)
{
	if (self->facingRight)
		self->spr.flags &= ~0x10;
	else
		self->spr.flags |= 0x10;
	Obj_ChangeAnim(self, CODEPTR(Tbl_Anim_1206E6));
	AI_NEXT_STEP(self, self->ai, 90);
}
