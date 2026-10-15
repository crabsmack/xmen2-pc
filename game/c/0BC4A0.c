/* SavageBio_ActFrame4F - SAVAGEBIGBIO script action: frame $4F with sound $F2, go on with the
 * script in 10 frames.
 *
 * self: the boss (a step of its AI script). SavageBio_ActFrame4FB is the same code. */
#include "xmen2.h"

void SavageBio_ActFrame4F(SavageBigBio *self)
{
	(*self->cls->method[20])(self, 0x4F);		/* SetFrame */
	Snd_Stop(0xF2);
	Snd_Play(0xF2);
	AI_NEXT_STEP(self, self->ai, 10);
}
