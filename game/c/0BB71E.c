/* SavageBio_ActExtendShort - SAVAGEBIGBIO script action: shoot a short tentacle out along
 * Tbl_SavageBioShortTentaclePath (frame $42, sound $F0), one segment per frame up to 10
 * (SavageBio_StateExtend10).
 *
 * self: the boss (a step of its AI script).
 * Returns self->flags & 8. */
#include "xmen2.h"

short SavageBio_ActExtendShort(SavageBigBio *self)
{
	self->anim = 0;
	(*self->cls->method[20])(self, 0x42);		/* SetFrame */
	Snd_Stop(0xF0);
	Snd_Play(0xF0);
	self->tentacle = (PathPt *)CODEPTR(Tbl_SavageBioShortTentaclePath);
	if (!self->tentacleLen)
		self->tentacleLen = 1;
	SET_STATE(self, SavageBio_StateExtend10);
	return self->flags & 8;
}
