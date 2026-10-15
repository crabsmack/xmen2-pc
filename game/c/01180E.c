/* BigAcolyte_ActWait30 - BIGACOLYTE fight script action: the next step of the script in 30 frames.
 *
 * self: the boss (never runs in the corpus). */
#include "xmen2.h"

void BigAcolyte_ActWait30(register BigAcolyte *self)
{
	AI_NEXT_STEP(self, self->ai, 30);
}
