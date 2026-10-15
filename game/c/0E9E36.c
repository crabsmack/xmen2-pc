/* Hologram_WaitLowerLiftDown - HOLOGRAM state: once the lower lift has stopped (its flags6 bit
 * $4000), wait for the players to enter the arena (Hologram_WaitPlayersInArena).
 *
 * self: the hologram.
 * Returns self->flags & 8 once the lift is down, nothing while waiting. */
#include "xmen2.h"

short Hologram_WaitLowerLiftDown(register Hologram *self)
{
	if (self->lowerLift->flags6 & 0x4000) {
		SET_STATE(self, (StateFn)Hologram_WaitPlayersInArena);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
