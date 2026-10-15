/* SavageBio_ActFrame4FB - SAVAGEBIGBIO script action: frame $4F with sound $F2, go on with the
 * script in 10 frames.
 *
 * self: the boss (a step of its AI script). A second copy of SavageBio_ActFrame4F (its own
 * jump-table entry, used by another script step). */
#include "xmen2.h"

void SavageBio_ActFrame4FB(SavageBigBio *self)
{
	(*self->cls->method[20])(self, 0x4F);		/* SetFrame */
	Snd_Stop(0xF2);
	Snd_Play(0xF2);
	AI_NEXT_STEP(self, self->ai, 10);
}
