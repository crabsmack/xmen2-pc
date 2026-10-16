/* EvilPsylocke_StateDashEnd - EVILPSYLOCKE state at the end of EvilPsylocke_ActDash's animation:
 * stop and go on with the AI script now.
 *
 * self: the clone. */
#include "xmen2.h"

void EvilPsylocke_StateDashEnd(EvilPsylocke *self)
{
	STOP_MOVING(self);
	AI_NEXT_STEP(self, self->ai, 0);
}
