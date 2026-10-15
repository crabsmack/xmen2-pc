/* SavageBio_ActExtendTentacle2 - SAVAGEBIGBIO script action: shoot the tentacle out along the
 * straight path Tbl_SavageBioStraightTentaclePath (frame $42, no sound), one segment per
 * frame (SavageBio_StateExtend).
 *
 * self: the boss (a step of its AI script; never runs in the corpus).
 * Returns self->flags & 8. */
#include "xmen2.h"

short SavageBio_ActExtendTentacle2(SavageBigBio *self)
{
	self->anim = 0;
	(*self->cls->method[20])(self, 0x42);		/* SetFrame */
	self->tentacle = (PathPt *)CODEPTR(Tbl_SavageBioStraightTentaclePath);
	if (!self->tentacleLen)
		self->tentacleLen = 1;
	SET_STATE(self, SavageBio_StateExtend);
	return self->flags & 8;
}
