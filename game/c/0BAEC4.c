/* SavageBio_ActScript2052 - SAVAGEBIGBIO script action: switch to the AI script at $0B7E54
 * (SavageBio_Script3) and start it at its first step now.
 *
 * self: the boss (a step of its AI script). */
#include "xmen2.h"

void SavageBio_ActScript2052(SavageBigBio *self)
{
	AiScript_Set(&self->ai, CODEPTR(Tbl_SavageBio_Script3));
	AI_FIRST_STEP(self, self->ai, 0);
}
