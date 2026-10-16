/* Apocalypse_StateWaitArrive - APOCALYPSE state: once its damped move has arrived (moveFlags
 * bit 4 clear), go on with the AI script on the next update.
 *
 * self: the boss. */
#include "xmen2.h"

void Apocalypse_StateWaitArrive(register Apocalypse *self)
{
	if (!(self->moveFlags & 0x10)) {
		AI_NEXT_STEP(self, self->ai, 0);
	}
}
