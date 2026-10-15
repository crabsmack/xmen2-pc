/* Apocalypse_StateDropStart - APOCALYPSE state: stop (the move and all motion), face the target
 * and drop onto it in 5 frames (Apocalypse_StateDrop).
 *
 * self: the boss. Returns self->flags & 8. */
#include "xmen2.h"

short Apocalypse_StateDropStart(register Apocalypse *self)
{
	self->moveFlags &= ~0x10;
	if (self->target->x < self->x)
		self->spr.flags |= 0x10;
	else
		self->spr.flags &= ~0x10;
	STOP_MOVING(self);
	SET_STATE_WAIT(self, Apocalypse_StateDrop, 5);
	return self->flags & 8;
}
