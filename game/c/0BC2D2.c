/* SavageBio_ActFrame40 - SAVAGEBIGBIO script action: stop animating, show frame $40 and go on
 * with the script now.
 *
 * self: the boss (a step of its AI script; never runs in the corpus). */
#include "xmen2.h"

void SavageBio_ActFrame40(SavageBigBio *self)
{
	self->anim = 0;
	(*self->cls->method[20])(self, 0x40);		/* SetFrame */
	AI_NEXT_STEP(self, self->ai, 0);
}
