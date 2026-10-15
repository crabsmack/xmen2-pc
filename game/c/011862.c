/* BigAcolyte_ActWait60 - BIGACOLYTE script action: the next step of the script in 60 frames.
 *
 * self: the boss. */
#include "xmen2.h"

void BigAcolyte_ActWait60(register BigAcolyte *self)
{
	AI_NEXT_STEP(self, self->ai, 60);
}
