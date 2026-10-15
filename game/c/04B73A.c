/* Apocalypse_ActStartBombs - APOCALYPSE script action: a bombing round. The bombs get faster
 * (+2, up to $180), the bomb class gets its speed and accel-timer settings
 * (ApocBomb_SetSpeed), the current bomb pattern is selected and Apocalypse_StateDropBombs drops
 * it from the next update.
 *
 * self: the boss. Returns self->flags & 8. */
#include "xmen2.h"

short Apocalypse_ActStartBombs(register Apocalypse *self)
{
	if (self->bombSpeed < 0x180)
		self->bombSpeed += 2;
	ApocBomb_SetSpeed(self->bombPeriod, self->bombSpeed, self->bombAccel);
	Apocalypse_SelectBombPattern(self);
	SET_STATE(self, Apocalypse_StateDropBombs);
	return self->flags & 8;
}
