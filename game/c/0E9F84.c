/* Hologram_WaitLowerLiftBoarded2 - HOLOGRAM state after the fight: once every live player stands
 * on the lower lift, send it up and wait for it to stop (Hologram_WaitLowerLiftUp).
 *
 * self: the hologram.
 * Returns self->flags & 8 once the lift goes, nothing while waiting. */
#include "xmen2.h"

short Hologram_WaitLowerLiftBoarded2(register Hologram *self)
{
	if (CoreElevator_CarriesAllPlayers(self->lowerLift)) {
		CoreElevator_SendUp(self->lowerLift);
		SET_STATE(self, Hologram_WaitLowerLiftUp);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
