/* Apocalypse_StateDropFall - APOCALYPSE state: once below y 224 (off the bottom), stop and go on
 * with the AI script in 255 frames.
 *
 * self: the boss. */
#include "xmen2.h"

void Apocalypse_StateDropFall(register Apocalypse *self)
{
	register short y = self->y >> 8;

	if (y > 0xE0) {
		STOP_MOVING(self);
		AI_NEXT_STEP(self, self->ai, 0xFF);
	}
}
