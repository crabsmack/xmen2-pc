/* Apocalypse_StateDefeatedRise - APOCALYPSE defeated: accelerate upwards for 30 frames, then
 * Apocalypse_StateVanish.
 *
 * self: the boss. Returns self->flags & 8. */
#include "xmen2.h"

short Apocalypse_StateDefeatedRise(register Apocalypse *self)
{
	self->ay = -0x20;
	SET_STATE_WAIT(self, (StateFn)Apocalypse_StateVanish, 30);
	return self->flags & 8;
}
