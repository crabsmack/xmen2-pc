/* FloatBio_ActWaitParam - FLOATBIO AI-script action (FloatBio_Script3): the next step after its
 * wait (waitTime).
 *
 * self: the object. */
#include "xmen2.h"

void FloatBio_ActWaitParam(FloatBio *self)
{
	AI_NEXT_STEP(self, self->ai, self->waitTime);
}
