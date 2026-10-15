/* SavageBio_StateNextStep - SAVAGEBIGBIO state: go on with the script now.
 *
 * self: the boss (after SavageBio_StateRetract).
 * Returns nothing in particular. */
#include "xmen2.h"

short SavageBio_StateNextStep(SavageBigBio *self)
{
	AI_NEXT_STEP(self, self->ai, 0);
	NO_RETURN_VALUE
}
