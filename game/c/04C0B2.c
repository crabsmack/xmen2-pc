/* Apocalypse_StateLungeEnd - APOCALYPSE state: end the lunge (the move and all motion stop) and
 * go on with the AI script on the next update.
 *
 * self: the boss. */
#include "xmen2.h"

void Apocalypse_StateLungeEnd(register Apocalypse *self)
{
	self->moveFlags &= ~0x10;
	STOP_MOVING(self);
	AI_NEXT_STEP(self, self->ai, 0);
}
