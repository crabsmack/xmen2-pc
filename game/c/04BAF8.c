/* Apocalypse_ActRotateTo0 - APOCALYPSE script action: set the circling angle to 4 and circle
 * until it is back at 0 (Apocalypse_StateRotateTo0 from the next update).
 *
 * self: the boss (never runs in the corpus). Returns self->flags & 8. */
#include "xmen2.h"

short Apocalypse_ActRotateTo0(register Apocalypse *self)
{
	self->angle = 4;
	SET_STATE(self, Apocalypse_StateRotateTo0);
	return self->flags & 8;
}
