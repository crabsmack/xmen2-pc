/* SavageBio_ActExtendTentacle - SAVAGEBIGBIO script action: shoot the tentacle out along the
 * long path Tbl_SavageBioTentaclePath (frame $42, sound $F0), one segment per frame
 * (SavageBio_StateExtend).
 *
 * self: the boss (a step of its AI script; never runs in the corpus).
 * Returns self->flags & 8. */
#include "xmen2.h"

short SavageBio_ActExtendTentacle(SavageBigBio *self)
{
	self->anim = 0;
	(*self->cls->method[20])(self, 0x42);		/* SetFrame */
	Snd_Stop(0xF0);
	Snd_Play(0xF0);
	self->tentacle = (PathPt *)CODEPTR(Tbl_SavageBioTentaclePath);
	if (!self->tentacleLen)
		self->tentacleLen = 1;
	SET_STATE(self, SavageBio_StateExtend);
	return self->flags & 8;
}
