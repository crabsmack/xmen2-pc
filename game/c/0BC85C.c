/* SavageBio_StateDying - SAVAGEBIGBIO state: dying - switch to the death script at $0B7E2C
 * (SavageBio_Script1) and start it now.
 *
 * self: the boss (set by SavageBigBio_TakeDamage when its health runs out).
 * Returns nothing in particular. */
#include "xmen2.h"

short SavageBio_StateDying(SavageBigBio *self)
{
	AiScript_Set(&self->ai, CODEPTR(Tbl_SavageBio_Script1Len));
	AI_FIRST_STEP(self, self->ai, 0);
	NO_RETURN_VALUE
}
