/* Hologram_WaitLowerLiftUp - HOLOGRAM state: once the lower lift has stopped at the top (its
 * flags6 bit $4000), wait for the players on the upper lift (Hologram_WaitUpperLiftBoarded).
 *
 * self: the hologram.
 * Returns self->flags & 8 once the lift is up, nothing while waiting. */
#include "xmen2.h"

short Hologram_WaitLowerLiftUp(register Hologram *self)
{
	if (self->lowerLift->flags6 & 0x4000) {
		SET_STATE(self, Hologram_WaitUpperLiftBoarded);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
