/* EvilMaker_ActWait60 - EVILMAKER AI script step: go on with the next step in 60 frames.
 *
 * self: EVILMAKER. Never runs in the corpus. */
#include "xmen2.h"

void EvilMaker_ActWait60(EvilMaker *self)
{
	AI_NEXT_STEP(self, self->ai, 0x3C);
}
