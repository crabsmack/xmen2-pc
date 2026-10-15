/* Apocalypse_StateWaitEntrance - APOCALYPSE state: once its entrance move has arrived, stop
 * sound $79 and all motion, set the circling angle to 180 and go on with the AI script now.
 *
 * self: the boss. */
#include "xmen2.h"

void Apocalypse_StateWaitEntrance(register Apocalypse *self)
{
	if (!(self->moveFlags & 0x10)) {
		Snd_Stop(0x79);
		STOP_MOVING(self);
		self->angle = 180;
		AI_NEXT_STEP(self, self->ai, 0);
	}
}
