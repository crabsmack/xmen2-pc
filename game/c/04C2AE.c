/* Apocalypse_StateDefeatedWait - APOCALYPSE defeated: once at the centre, stop, end the animation
 * and rise in 120 frames (Apocalypse_StateDefeatedRise).
 *
 * self: the boss. Returns self->flags & 8. */
#include "xmen2.h"

short Apocalypse_StateDefeatedWait(register Apocalypse *self)
{
	if (!(self->moveFlags & 0x10)) {
		STOP_MOVING(self);
		self->anim = 0;
		SET_STATE_WAIT(self, Apocalypse_StateDefeatedRise, 120);
		return self->flags & 8;
	}
	SET_STATE(self, Apocalypse_StateDefeatedWait);
	return self->flags & 8;
}
